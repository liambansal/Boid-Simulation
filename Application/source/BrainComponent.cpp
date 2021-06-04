//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

// File's header.
#include "BrainComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"
#include "LearnOpenGL/shader.h"
#include "OctTree.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Utilities.h"

typedef std::vector<Entity*> pEntityVector;

float BrainComponent::ms_fSeparationForce = 0.4f;
float BrainComponent::ms_fAlignmentForce = 0.2f;
float BrainComponent::ms_fCohesionForce = 0.6f;
float BrainComponent::ms_fWanderForce = 0.5f;

BrainComponent::BrainComponent(Entity* a_pOwner,
	Scene* a_pScene) : Component(a_pOwner),
	m_uiNeighbourCount(0),
	mc_fSpeed(1.0f),
	mc_fMaximumVelocity(1.5f),
	mc_fMaximumNeighbourDistance(4.0f),
	m_fLastUpdate(0.0f),
	m_currentVelocity(0.0f),
	m_behavioralVelocity(0.0f),
	m_collisionSeparationVelocity(0.0f),
	m_wanderPoint(0.0f),
	m_pScene(a_pScene),
	m_pEntityCollider(static_cast<ColliderComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_COLLIDER)))
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
	m_currentVelocity(a_rBrainToCopy.m_currentVelocity),
	m_behavioralVelocity(a_rBrainToCopy.m_behavioralVelocity),
	m_collisionSeparationVelocity(a_rBrainToCopy.m_collisionSeparationVelocity),
	m_wanderPoint(a_rBrainToCopy.m_wanderPoint),
	m_pScene(a_pScene),
	m_pEntityCollider(static_cast<ColliderComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_COLLIDER)))
{
	m_componentType = a_rBrainToCopy.m_componentType;
}

void BrainComponent::Update(float a_fDeltaTime)
{
	m_fLastUpdate += a_fDeltaTime;
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

	glm::vec3 currentPosition = (glm::vec3)pOwnerTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR);
	glm::vec3 forwardDirection = (glm::vec3)pOwnerTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_FORWARD_VECTOR);

	if (m_fLastUpdate >= updateStep)
	{
		m_fLastUpdate = 0.0f;
		// Calculate our boid's behaviour.
		CalculateBehaviouralVelocity(currentPosition,
			forwardDirection);
	}

	// Check for any collisions.
	if (m_pEntityCollider && m_pEntityCollider->IsColliding())
	{
		CalculateCollisionVelocity(currentPosition);
		m_currentVelocity = m_collisionSeparationVelocity;
	}
	else
	{
		// Reset collision separation velocity now that all collisions are over.
		m_collisionSeparationVelocity = glm::vec3(0.0f);
		m_currentVelocity = m_behavioralVelocity;
	}

	// Check if entity is out of scene bounds.
	if (m_pScene && !m_pScene->GetOctTree().GetBoundary().Contains(currentPosition))
	{
		// Make sure boids stay within the oct tree's bounds.
		m_currentVelocity = *m_pScene->GetOctTree().GetBoundary().GetPosition() - currentPosition;
		// Reset behaviour after reaching scene bounds.
		m_behavioralVelocity = glm::vec3(0.0f);
	}

	m_currentVelocity = glm::clamp(m_currentVelocity,
		glm::vec3(-mc_fMaximumVelocity, -mc_fMaximumVelocity, -mc_fMaximumVelocity),
		glm::vec3(mc_fMaximumVelocity, mc_fMaximumVelocity, mc_fMaximumVelocity));
	currentPosition += m_currentVelocity * a_fDeltaTime;
	// Update the entity's transform matrix.
	UpdateMatrix(pOwnerTransform,
		&currentPosition,
		&m_currentVelocity);
}

// Calculates a point to move towards.
glm::vec3 BrainComponent::CalculateSeekVelocity(const glm::vec3& a_rTargetPosition,
	const glm::vec3& a_rCurrentPosition) const
{
	glm::vec3 targetDirection(a_rTargetPosition - a_rCurrentPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 seekVelocity = targetDirection * mc_fSpeed - m_currentVelocity;
	return seekVelocity;
}

// Calculates a point to move away from.
glm::vec3 BrainComponent::CalculateFleeVelocity(const glm::vec3& a_rTargetPosition,
	const glm::vec3& a_rCurrentPosition) const
{
	glm::vec3 targetDirection(a_rCurrentPosition - a_rTargetPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 fleeVelocity = (targetDirection * mc_fSpeed) - m_currentVelocity;
	return fleeVelocity;
}

// Calculates a semi-random point to move towards.
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

// Calculate velocity to move boids away from each other.
glm::vec3 BrainComponent::CalculateSeparationVelocity(glm::vec3 a_separationVelocity,
	glm::vec3 a_targetVector,
	unsigned int a_uiNeighbourCount)
{
	a_separationVelocity += a_targetVector;

	if (glm::length(a_separationVelocity) > 0.0f &&
		a_uiNeighbourCount > 0)
	{
		// Average the separation force
		a_separationVelocity /= a_uiNeighbourCount;
		// Only normalise vector with length greater than zero
		a_separationVelocity = glm::normalize(a_separationVelocity);
	}

	return a_separationVelocity;
}

// Calculate velocity to move boids alongside each other.
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

// Calculate velocity to move boids towards each other.
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

// Calculates the boids flocking behaviours.
void BrainComponent::CalculateBehaviouralVelocity(glm::vec3& a_rEntityPosition,
	glm::vec3& a_rEntityForward)
{
	// Get the component's owner entity.
	const Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return;
	}

	m_uiNeighbourCount = 0;
	pEntityVector nearbyEntities;
	// Get all nearby entities.
	m_pScene->GetOctTree().Query(Boundary<glm::vec3>(a_rEntityPosition,
		glm::vec3(mc_fMaximumNeighbourDistance)),
		nearbyEntities);
	m_uiNeighbourCount = nearbyEntities.size();
	// Behavioural forces.
	glm::vec3 seperationVelocity(0.0f);
	glm::vec3 alignmentVelocity(0.0f);
	glm::vec3 cohesionVelocity(0.0f);

	// Loop over all entities in scene.
	for (const Entity* entity : nearbyEntities)
	{
		if (!entity)
		{
			continue;
		}

		// Make sure we haven't found this entity and it's a boid.
		if (entity->GetID() != pOwnerEntity->GetID() && entity->GetTag() == "Boid")
		{
			const TransformComponent* ptargetTransform = static_cast<TransformComponent*>(entity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
			const BrainComponent* pTargetBrain = static_cast<BrainComponent*>(entity->GetComponentOfType(COMPONENT_TYPE_BRAIN));

			if (!ptargetTransform || !pTargetBrain)
			{
				continue;
			}

			// Find distance to iterator entity
			const glm::vec3 targetPosition = (const glm::vec3)ptargetTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR);
			seperationVelocity = CalculateSeparationVelocity(seperationVelocity,
				// Don't want positions to have the same value.
				a_rEntityPosition == targetPosition ? GetRandomNearbyPoint(a_rEntityPosition) - a_rEntityPosition : a_rEntityPosition - targetPosition,
				m_uiNeighbourCount);
			alignmentVelocity = CalculateAlignmentVelocity(alignmentVelocity,
				pTargetBrain->GetVelocity());
			cohesionVelocity = CalculateCohesionVelocity(cohesionVelocity,
				targetPosition,
				a_rEntityPosition);
		}
	}

	seperationVelocity *= ms_fSeparationForce;
	alignmentVelocity *= ms_fAlignmentForce;
	cohesionVelocity *= ms_fCohesionForce;
	glm::vec3 wanderVelocity = CalculateWanderVelocity(a_rEntityForward, a_rEntityPosition) * ms_fWanderForce;
	glm::vec3 newForce(wanderVelocity + cohesionVelocity + alignmentVelocity + seperationVelocity);
	m_behavioralVelocity += newForce;
}

// Calculates the separation velocity produced by collisions.
void BrainComponent::CalculateCollisionVelocity(const glm::vec3 a_entityPosition)
{
	for (std::vector<ColliderComponent*>::const_iterator iterator = m_pEntityCollider->GetCollisions().begin();
		iterator != m_pEntityCollider->GetCollisions().end();
		++iterator)
	{
		Entity* pCollidingEntity = (*iterator)->GetEntity();

		if (!pCollidingEntity)
		{
			return;
		}

		TransformComponent* pCollidingEntityTransform = static_cast<TransformComponent*>(pCollidingEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

		if (!pCollidingEntityTransform)
		{
			return;
		}

		glm::vec3 targetVector = (a_entityPosition == *pCollidingEntityTransform->GetPosition()) ?
			// Get nearby point encase entities spawn on top of each other.
			GetRandomNearbyPoint(a_entityPosition) - a_entityPosition :
			// Normal collision avoidance behaviour.
			a_entityPosition - *pCollidingEntityTransform->GetPosition();
		// Calculate where to move towards.
		m_collisionSeparationVelocity = CalculateSeparationVelocity(m_collisionSeparationVelocity,
			targetVector,
			m_pEntityCollider->GetCollisions().size());
	}
}

void BrainComponent::UpdateMatrix(TransformComponent* a_pTransform,
	glm::vec3* a_pPosition,
	glm::vec3* a_pForward)
{
	if (!a_pTransform || !a_pPosition || !a_pForward)
	{
		return;
	}

	if (glm::length(*a_pForward) > 0.0f)
	{
		*a_pForward = glm::normalize(*a_pForward);
	}

	glm::vec3 upDirection = (glm::vec3)a_pTransform->GetMatrixRow(TransformComponent::MATRIX_ROW_UP_VECTOR);
	upDirection -= (*a_pForward) * glm::dot(*a_pForward, upDirection);

	if (glm::length(upDirection) > 0.0f)
	{
		glm::normalize(upDirection);
	}

	glm::vec3 rightDirection = glm::cross(upDirection, *a_pForward);

	if (glm::length(rightDirection) > 0.0f)
	{
		glm::normalize(rightDirection);
	}

	// Update transform matrix.
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_UP_VECTOR, upDirection);
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_RIGHT_VECTOR, rightDirection);
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_FORWARD_VECTOR, *a_pForward);
	a_pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR, *a_pPosition);
}

// Gets a semi-random position, based around the argument position.
glm::vec3 BrainComponent::GetRandomNearbyPoint(glm::vec3 a_originPosition) const
{
	const float absoluteRange = 1.0f;
	return glm::vec3(a_originPosition.x + Utilities::RandomRange(-absoluteRange, absoluteRange),
		a_originPosition.y + Utilities::RandomRange(-absoluteRange, absoluteRange),
		a_originPosition.z + Utilities::RandomRange(-absoluteRange, absoluteRange));
}