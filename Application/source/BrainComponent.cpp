//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

// File's header.
#include "BrainComponent.h"
#include "Entity.h"
#include "LearnOpenGL/shader.h"
#include "OctTree.h"
#include "Scene.h"
#include "TransformComponent.h"

typedef std::vector<Entity*> pEntityVector;

float BrainComponent::ms_fSeparationForce = 0.4f;
float BrainComponent::ms_fAlignmentForce = 0.2f;
float BrainComponent::ms_fCohesionForce = 0.6f;
float BrainComponent::ms_fWanderForce = 0.5f;

BrainComponent::BrainComponent(Entity* a_pOwner,
	Scene* a_pScene) : Component(a_pOwner),
	m_uiNeighbourCount(0),
	mc_fSpeed(1.0f),
	mc_fMaximumVelocity(2.0f),
	mc_fMaximumNeighbourDistance(1.5f),
	m_fLastUpdate(0.0f),
	m_velocity(0.0f),
	m_wanderPoint(0.0f),
	m_pScene(a_pScene)
{
	m_componentType = COMPONENT_TYPE_BRAIN;
}

BrainComponent::BrainComponent(Entity* a_pOwner,
	BrainComponent& a_rBrainToCopy,
	Scene* a_pScene) : Component(a_pOwner),
	mc_fSpeed(a_rBrainToCopy.mc_fSpeed),
	mc_fMaximumVelocity(a_rBrainToCopy.mc_fMaximumVelocity),
	mc_fMaximumNeighbourDistance(a_rBrainToCopy.mc_fMaximumNeighbourDistance),
	m_uiNeighbourCount(a_rBrainToCopy.m_uiNeighbourCount),
	m_fLastUpdate(a_rBrainToCopy.m_fLastUpdate),
	m_velocity(a_rBrainToCopy.m_velocity),
	m_wanderPoint(a_rBrainToCopy.m_wanderPoint),
	m_pScene(a_pScene)
{
	m_componentType = a_rBrainToCopy.m_componentType;
}

void BrainComponent::Update(float a_deltaTime)
{
	m_fLastUpdate += a_deltaTime;
	const float updateStep = 0.15f;

	// Get this components owner entity.
	Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return;
	}

	TransformComponent* pOwnerTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pOwnerTransform)
	{
		return;
	}

	glm::vec3 forwardDirection = (glm::vec3)pOwnerTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_FORWARD_VECTOR);
	glm::vec3 currentPosition = (glm::vec3)pOwnerTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR);

	if (m_fLastUpdate >= updateStep)
	{
		m_fLastUpdate = 0.0f;
		glm::vec3 seperationVelocity(0.0f);
		glm::vec3 alignmentVelocity(0.0f);
		glm::vec3 cohesionVelocity(0.0f);
		CalculateBehaviouralVelocities(seperationVelocity,
			alignmentVelocity,
			cohesionVelocity);
		seperationVelocity *= ms_fSeparationForce;
		alignmentVelocity *= ms_fAlignmentForce;
		cohesionVelocity *= ms_fCohesionForce;
		glm::vec3 wanderVelocity = CalculateWanderVelocity(forwardDirection, currentPosition) * ms_fWanderForce;
		glm::vec3 newForce(wanderVelocity + cohesionVelocity + alignmentVelocity + seperationVelocity);
		m_velocity += newForce;
	}
	
	m_velocity = glm::clamp(m_velocity,
		glm::vec3(-mc_fMaximumVelocity, -mc_fMaximumVelocity, -mc_fMaximumVelocity),
		glm::vec3(mc_fMaximumVelocity, mc_fMaximumVelocity, mc_fMaximumVelocity));
	currentPosition += m_velocity * a_deltaTime;
	forwardDirection = m_velocity;

	if (glm::length(forwardDirection) > 0.0f)
	{
		forwardDirection = glm::normalize(forwardDirection);
	}

	glm::vec3 upDirection = (glm::vec3)pOwnerTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_UP_VECTOR);
	upDirection -= forwardDirection * glm::dot(forwardDirection, upDirection);
	
	if (glm::length(upDirection) > 0.0f)
	{
		glm::normalize(upDirection);
	}

	glm::vec3 rightDirection = glm::cross(upDirection, forwardDirection);
	
	if (glm::length(rightDirection) > 0.0f)
	{
		glm::normalize(rightDirection);
	}
	
	// Update transform matrix.
	pOwnerTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_UP_VECTOR, upDirection);
	pOwnerTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_RIGHT_VECTOR, rightDirection);
	pOwnerTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_FORWARD_VECTOR, forwardDirection);
	pOwnerTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR, currentPosition);
}

glm::vec3 BrainComponent::CalculateSeekVelocity(const glm::vec3& a_rTargetPosition,
	const glm::vec3& a_rCurrentPosition) const
{
	glm::vec3 targetDirection(a_rTargetPosition - a_rCurrentPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 seekVelocity = targetDirection * mc_fSpeed - m_velocity;
	return seekVelocity;
}

glm::vec3 BrainComponent::CalculateFleeVelocity(const glm::vec3& a_rTargetPosition,
	const glm::vec3& a_rCurrentPosition) const
{
	glm::vec3 targetDirection(a_rCurrentPosition - a_rTargetPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 fleeVelocity = (targetDirection * mc_fSpeed) - m_velocity;
	return fleeVelocity;
}

glm::vec3 BrainComponent::CalculateWanderVelocity(const glm::vec3& a_rForwardDirection,
	const glm::vec3& a_rCurrentPosition)
{
	// Greater values result in wider turning angles.
	const float projectDistance = 4.0f;
	// Project a point in front for the center of a sphere.
	glm::vec3 sphereOrigin = a_rCurrentPosition + a_rForwardDirection * projectDistance;
	const float jitter = 0.5f;
	// Effects radius of sphere to cast forward.
	const float wanderRadius = 2.0f;

	if (glm::length(m_wanderPoint) == 0.0f)
	{
		// Find a random point on a forward casted sphere.
		glm::vec3 randomPoint = glm::sphericalRand(wanderRadius);
		// Add random point to sphere origin
		m_wanderPoint = (sphereOrigin + randomPoint);
	}
	
	// Direction to move along
	glm::vec3 targetDirection = glm::normalize(m_wanderPoint - sphereOrigin) * wanderRadius;
	// Calculate final target position
	m_wanderPoint = sphereOrigin + targetDirection;
	// Add jitter to movement
	m_wanderPoint += glm::sphericalRand(jitter);
	// Seek to the wander point
	return CalculateSeekVelocity(m_wanderPoint, a_rCurrentPosition);
}

glm::vec3 BrainComponent::CalculateSeparationVelocity(glm::vec3 a_separationVelocity,
	glm::vec3 a_targetVector)
{
	a_separationVelocity += a_targetVector;

	if (glm::length(a_separationVelocity) > 0.0f &&
		m_uiNeighbourCount > 0)
	{
		// Average the separation force
		a_separationVelocity /= m_uiNeighbourCount;
		// Only normalise vector with length greater than zero
		a_separationVelocity = glm::normalize(a_separationVelocity);
	}

	return a_separationVelocity;
}

glm::vec3 BrainComponent::CalculateAlignmentVelocity(glm::vec3 a_alignmentVelocity,
	glm::vec3 a_targetVector)
{
	// Add neighbours' velocities
	a_alignmentVelocity += a_targetVector;

	if (glm::length(a_alignmentVelocity) > 0.0f &&
		m_uiNeighbourCount > 0)
	{
		a_alignmentVelocity /= m_uiNeighbourCount;
		a_alignmentVelocity = glm::normalize(a_alignmentVelocity);
	}

	return a_alignmentVelocity;
}

glm::vec3 BrainComponent::CalculateCohesionVelocity(glm::vec3 a_cohesionVelocity,
	glm::vec3 a_targetPosition,
	glm::vec3 a_localPosition)
{
	a_cohesionVelocity += a_targetPosition;

	if (glm::length(a_cohesionVelocity) > 0.0f &&
		m_uiNeighbourCount > 0)
	{
		a_cohesionVelocity /= m_uiNeighbourCount;
		a_cohesionVelocity = glm::normalize(a_cohesionVelocity - a_localPosition);
	}

	return a_cohesionVelocity;
}

void BrainComponent::CalculateBehaviouralVelocities(glm::vec3& a_rSeparationVelocity,
	glm::vec3& a_rAlignmentVelocity,
	glm::vec3& a_rCohesionVelocity)
{
	// Get the component's owner entity.
	const Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return;
	}

	// Get this entities transform.
	const TransformComponent* pEntityTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pEntityTransform)
	{
		return;
	}

	// Get entity position.
	const glm::vec3 localPosition = (const glm::vec3)pEntityTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR);
	m_uiNeighbourCount = 0;
	pEntityVector nearbyEntities;
	// Get all nearby entities.
	m_pScene->GetOctTree().Query(Boundary(localPosition,
		glm::vec3(mc_fMaximumNeighbourDistance)),
		nearbyEntities);
	m_uiNeighbourCount = nearbyEntities.size();

	// Loop over all entities in scene.
	for (pEntityVector::const_iterator iterator = nearbyEntities.begin(); iterator != nearbyEntities.end(); ++iterator)
	{
		const Entity* pTargetEntity = *iterator;

		if (!pTargetEntity)
		{
			continue;
		}

		// Make sure we haven't found this entity and it's a boid.
		if (pTargetEntity->GetID() != pOwnerEntity->GetID() && pTargetEntity->GetTag() == "Boid")
		{
			const TransformComponent* ptargetTransform = static_cast<TransformComponent*>(pTargetEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
			const BrainComponent* pTargetBrain = static_cast<BrainComponent*>(pTargetEntity->GetComponentOfType(COMPONENT_TYPE_BRAIN));

			if (!ptargetTransform || !pTargetBrain)
			{
				continue;
			}

			// Find distance to iterator entity
			const glm::vec3 targetPosition = (const glm::vec3)ptargetTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR);
			a_rSeparationVelocity = CalculateSeparationVelocity(a_rSeparationVelocity, localPosition - targetPosition);
			a_rAlignmentVelocity = CalculateAlignmentVelocity(a_rAlignmentVelocity, pTargetBrain->GetVelocity());
			a_rCohesionVelocity = CalculateCohesionVelocity(a_rCohesionVelocity, targetPosition, localPosition);
		}
	}
}
