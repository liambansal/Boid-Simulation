//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#include "BrainComponent.h" // File's header.
#include "Entity.h"
#include "LearnOpenGL/shader.h"
#include "TransformComponent.h"

BrainComponent::BrainComponent(Entity* a_pOwner) : Component(a_pOwner),
	mc_fSpeed(1.0f),
	mc_fMaximumVelocity(2.0f),
	m_velocity(0.0f),
	m_wanderPoint(0.0f)
{
	m_componentType = COMPONENT_TYPE_AI;
}

void BrainComponent::Update(float a_deltaTime)
{
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
	glm::vec3 seperationVelocity = CalculateSeparationVelocity() * 0.4f;
	glm::vec3 alignmentVelocity = CalculateAlignmentVelocity() * 0.2f;
	glm::vec3 cohesionVelocity = CalculateCohesionVelocity() * 0.5f;
	glm::vec3 wanderVelocity = CalculateWanderVelocity(forwardDirection, currentPosition) * 0.6f;
	glm::vec3 newForce(wanderVelocity + cohesionVelocity + alignmentVelocity + seperationVelocity);
	
	// Apply force.
	m_velocity += newForce;
	m_velocity = glm::clamp(m_velocity,
		glm::vec3(-mc_fMaximumVelocity, -mc_fMaximumVelocity, -mc_fMaximumVelocity),
		glm::vec3(mc_fMaximumVelocity, mc_fMaximumVelocity, mc_fMaximumVelocity));
	currentPosition += m_velocity * a_deltaTime;

	// Update transform matrix rows.
	forwardDirection = m_velocity;

	if (glm::length(forwardDirection) > 0.0f)
	{
		forwardDirection = glm::normalize(forwardDirection);
	}

	glm::vec3 upDirection = pOwnerTransform->GetMatrix()[MATRIX_ROW_UP_VECTOR];
	glm::vec3 rightDirection = glm::cross(upDirection, forwardDirection);
	// Update transform matrix.
	pOwnerTransform->SetPosition(MATRIX_ROW_RIGHT_VECTOR, rightDirection);
	pOwnerTransform->SetPosition(MATRIX_ROW_FORWARD_VECTOR, forwardDirection);
	pOwnerTransform->SetPosition(MATRIX_ROW_POSITION_VECTOR, currentPosition);
}

void BrainComponent::Draw(Shader* a_pShader)
{}

glm::vec3 BrainComponent::CalculateSeekVelocity(const glm::vec3& a_targetPosition,
	const glm::vec3& a_currentPosition) const
{
	glm::vec3 targetDirection(a_targetPosition - a_currentPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 seekVelocity = (targetDirection * mc_fSpeed) - m_velocity;
	return seekVelocity;
}

glm::vec3 BrainComponent::CalculateFleeVelocity(const glm::vec3& a_targetPosition,
	const glm::vec3& a_currentPosition) const
{
	glm::vec3 targetDirection(a_currentPosition - a_targetPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 fleeVelocity = (targetDirection * mc_fSpeed) - m_velocity;
	return fleeVelocity;
}

glm::vec3 BrainComponent::CalculateWanderVelocity(const glm::vec3& a_forwardDirection,
	const glm::vec3& a_currentPosition)
{
	// Greater values result in wider turning angles.
	const float projectDistance = 2.0f;
	// Project a point in front for the center of a sphere.
	glm::vec3 sphereOrigin = a_currentPosition + a_forwardDirection * projectDistance;
	const float jitter = 0.5f;
	// Effects radius of sphere to cast forward.
	const float wanderRadius = 4.0f;

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
	return CalculateSeekVelocity(m_wanderPoint, a_currentPosition);
}

glm::vec3 BrainComponent::CalculateSeparationVelocity()
{
	// Get the component's owner entity.
	Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return glm::vec3(0.0f);
	}

	// Get this entities transform.
	TransformComponent* pEntityTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pEntityTransform)
	{
		return glm::vec3(0.0f);
	}

	// Final vector to return
	glm::vec3 separationVelocity(0.0f);
	// Get entity position.
	glm::vec3 localPosition = pEntityTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
	// Get the scene's entities.
	const std::map<const unsigned int, Entity*>& entityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator iterator = entityMap.begin();
	unsigned int neighbourCount = 0;

	// Loop over all entities in scene.
	for (iterator; iterator != entityMap.end(); ++iterator)
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

			if (!ptargetTransform)
			{
				break;
			}

			// Find distance to iterator entity
			glm::vec3 targetPosition = ptargetTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
			float distance = glm::length(targetPosition - localPosition);
			// Check distance is within our neighbourhood.
			const float neighbourhoodRadius = 5.0f;

			if (distance < neighbourhoodRadius)
			{
				separationVelocity += localPosition - targetPosition;
				++neighbourCount;
			}
		}
	}

	if (glm::length(separationVelocity) > 0.0f)
	{
		// Average the separation force
		separationVelocity /= neighbourCount;
		// Only normalise vector with length greater than zero
		separationVelocity = glm::normalize(separationVelocity);
	}

	return separationVelocity;
}

glm::vec3 BrainComponent::CalculateAlignmentVelocity()
{
	// Get the component's owner entity.
	Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return glm::vec3(0.0f);
	}

	// Get this entities transform.
	TransformComponent* pEntityTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pEntityTransform)
	{
		return glm::vec3(0.0f);
	}

	glm::vec3 alignmentVelocity(0.0f);
	// Get entity position.
	glm::vec3 localPosition = pEntityTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
	// Get the scene's entities.
	const std::map<const unsigned int, Entity*>& entityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator iterator = entityMap.begin();
	unsigned int neighbourCount = 0;

	// Loop over all entities in scene.
	for (iterator; iterator != entityMap.end(); ++iterator)
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

			if (!ptargetTransform)
			{
				break;
			}

			// Find distance to iterator entity
			glm::vec3 targetPosition = ptargetTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
			float distance = glm::length(targetPosition - localPosition);
			// Check distance is within our neighbourhood.
			const float neighbourhoodRadius = 5.0f;

			if (distance < neighbourhoodRadius)
			{
				// Add neighbours' velocities
				alignmentVelocity += pTargetBrain->GetVelocity();
				++neighbourCount;
			}
		}
	}

	if (glm::length(alignmentVelocity) > 0.0f)
	{
		alignmentVelocity /= neighbourCount;
		alignmentVelocity = glm::normalize(alignmentVelocity);
	}

	return alignmentVelocity;
}

glm::vec3 BrainComponent::CalculateCohesionVelocity()
{
	// Get the component's owner entity.
	Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return glm::vec3(0.0f);
	}

	// Get this entities transform.
	TransformComponent* pEntityTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pEntityTransform)
	{
		return glm::vec3(0.0f);
	}

	// Get entity position.
	glm::vec3 localPosition = pEntityTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
	glm::vec3 cohesionVelocity(0.0f);
	// Get the scene's entities.
	const std::map<const unsigned int, Entity*>& entityMap = Entity::GetEntityMap();
	std::map<const unsigned int, Entity*>::const_iterator iterator = entityMap.begin();
	unsigned int neighbourCount = 0;

	// Loop over all entities in scene.
	for (iterator; iterator != entityMap.end(); ++iterator)
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

			if (!ptargetTransform)
			{
				break;
			}

			// Find distance to iterator entity
			glm::vec3 targetPosition = ptargetTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
			float distance = glm::length(targetPosition - localPosition);
			// Check distance is within our neighbourhood.
			const float neighbourhoodRadius = 5.0f;

			if (distance < neighbourhoodRadius)
			{
				cohesionVelocity += targetPosition;
				++neighbourCount;
			}
		}
	}

	if (glm::length(cohesionVelocity) > 0.0f)
	{
		cohesionVelocity /= neighbourCount;
		cohesionVelocity = glm::normalize(cohesionVelocity - localPosition);
	}

	return cohesionVelocity;
}
