//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#include "BrainComponent.h" // File's header.
#include "Entity.h"
#include "LearnOpenGL/shader.h"
#include "TransformComponent.h"

BrainComponent::BrainComponent(Entity* a_pOwner) : Component(a_pOwner),
	m_iNeighbourCount(0),
	mc_fSpeed(1.0f),
	mc_fMaximumVelocity(2.0f),
	m_fLastUpdate(0.0f),
	m_velocity(0.0f),
	m_wanderPoint(0.0f)
{
	m_componentType = COMPONENT_TYPE_AI;
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

	glm::vec3 forwardDirection = pOwnerTransform->GetMatrix()[MATRIX_ROW_FORWARD_VECTOR];
	glm::vec3 currentPosition = pOwnerTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];

	if (m_fLastUpdate >= updateStep)
	{
		m_fLastUpdate = 0.0f;
		glm::vec3 seperationVelocity(0.0f);
		glm::vec3 alignmentVelocity(0.0f);
		glm::vec3 cohesionVelocity(0.0f);
		CalculateBehaviouralVelocities(seperationVelocity, alignmentVelocity, cohesionVelocity);
		seperationVelocity *= 0.4f;
		alignmentVelocity *= 0.2f;
		cohesionVelocity *= 0.6f;
		glm::vec3 wanderVelocity = CalculateWanderVelocity(forwardDirection, currentPosition) * 0.5f;
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

	glm::vec3 upDirection = pOwnerTransform->GetMatrix()[MATRIX_ROW_UP_VECTOR];
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
	pOwnerTransform->SetPosition(MATRIX_ROW_UP_VECTOR, upDirection);
	pOwnerTransform->SetPosition(MATRIX_ROW_RIGHT_VECTOR, rightDirection);
	pOwnerTransform->SetPosition(MATRIX_ROW_FORWARD_VECTOR, forwardDirection);
	pOwnerTransform->SetPosition(MATRIX_ROW_POSITION_VECTOR, currentPosition);
}

void BrainComponent::Draw(Shader* a_pShader)
{}

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
		m_iNeighbourCount > 0)
	{
		// Average the separation force
		a_separationVelocity /= m_iNeighbourCount;
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
		m_iNeighbourCount > 0)
	{
		a_alignmentVelocity /= m_iNeighbourCount;
		a_alignmentVelocity = glm::normalize(a_alignmentVelocity);
	}

	return a_alignmentVelocity;
}

glm::vec3 BrainComponent::CalculateCohesionVelocity(glm::vec3 a_cohesionVelocity,
	glm::vec3 a_targetVector,
	glm::vec3 a_localPosition)
{
	// Get the component's owner entity.
	const Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return glm::vec3(0.0f);
	}

	// Get this entities transform.
	const TransformComponent* pEntityTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pEntityTransform)
	{
		return glm::vec3(0.0f);
	}

	// Get entity position.
	const glm::vec3 localPosition = pEntityTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];

	a_cohesionVelocity += a_targetVector;

	if (glm::length(a_cohesionVelocity) > 0.0f &&
		m_iNeighbourCount > 0)
	{
		a_cohesionVelocity /= m_iNeighbourCount;
		a_cohesionVelocity = glm::normalize(a_cohesionVelocity - localPosition);
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
	const glm::vec3 localPosition = pEntityTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
	// Get the scene's entities.
	const std::map<const unsigned int, Entity*>& rEntityMap = Entity::GetEntityMap();
	m_iNeighbourCount = 0;

	// Loop over all entities in scene.
	for (std::map<const unsigned int, Entity*>::const_iterator iterator = rEntityMap.begin(); iterator != rEntityMap.end(); ++iterator)
	{
		const Entity* pTargetEntity = iterator->second;

		if (!pTargetEntity)
		{
			continue;
		}

		// make sure we haven't found this entity.
		if (pTargetEntity->GetID() != pOwnerEntity->GetID())
		{
			const TransformComponent* ptargetTransform = static_cast<TransformComponent*>(pTargetEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
			const BrainComponent* pTargetBrain = static_cast<BrainComponent*>(pTargetEntity->GetComponentOfType(COMPONENT_TYPE_AI));

			if (!ptargetTransform || !pTargetBrain)
			{
				continue;
			}

			// Find distance to iterator entity
			const glm::vec3 targetPosition = ptargetTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
			const float distance = glm::length(targetPosition - localPosition);
			// Check distance is within our neighbourhood.
			const float neighbourhoodRadius = 10.0f;

			if (distance < neighbourhoodRadius)
			{
				++m_iNeighbourCount;
				a_rSeparationVelocity = CalculateSeparationVelocity(a_rSeparationVelocity, localPosition - targetPosition);
				a_rAlignmentVelocity = CalculateAlignmentVelocity(a_rAlignmentVelocity, pTargetBrain->GetVelocity());
				a_rCohesionVelocity = CalculateCohesionVelocity(a_rCohesionVelocity, targetPosition, localPosition);
			}
		}
	}
}