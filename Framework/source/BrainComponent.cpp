//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#include "BrainComponent.h" // File's header.
#include "Entity.h"
#include "LearnOpenGL/shader.h"
#include "TransformComponent.h"

BrainComponent::BrainComponent(Entity* a_pOwner) : Component(a_pOwner),
	m_fSpeed(3.0f),
	m_velocity(0.0f),
	m_wanderPoint(0.0f)
{
	m_componentType = COMPONENT_TYPE_AI;
}

void BrainComponent::Update(float a_deltaTime)
{
	// Get this components owner entity.
	Entity* pEntity = GetEntity();

	if (!pEntity)
	{
		return;
	}

	TransformComponent* pOwnerTransform = static_cast<TransformComponent*>(pEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pOwnerTransform)
	{
		return;
	}

	glm::vec3 forwardDirection = pOwnerTransform->GetMatrix()[MATRIX_ROW_FORWARD_VECTOR];
	glm::vec3 currentPosition = pOwnerTransform->GetMatrix()[MATRIX_ROW_POSITION_VECTOR];
	glm::vec3 newForce = glm::vec3(0.0f);
	// Seek.
	//newForce = CalculateSeekForce(glm::vec3(4.0f, 0.0f, 4.0f), currentPosition);
	// Flee.
	//newForce = CalculateFleeForce(glm::vec3(0.0f, 0.0f, 0.0f), currentPosition);
	// Wander.
	newForce = CalculateWanderForce(forwardDirection, currentPosition);

	// Apply force.
	m_velocity += newForce;
	// Clamp velocity
	const float maximumVelocity = 10.0f;
	m_velocity = glm::clamp(m_velocity,
		glm::vec3(-maximumVelocity, 0.0f, -maximumVelocity),
		glm::vec3(maximumVelocity, 0.0f, maximumVelocity));
	// Apply velocity to position.
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

glm::vec3 BrainComponent::CalculateSeekForce(const glm::vec3& a_targetPosition,
	const glm::vec3& a_currentPosition) const
{
	glm::vec3 targetDirection(a_targetPosition - a_currentPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 newVelocity = (targetDirection * m_fSpeed) - m_velocity;
	return newVelocity;
}

glm::vec3 BrainComponent::CalculateFleeForce(const glm::vec3& a_targetPosition,
	const glm::vec3& a_currentPosition) const
{
	glm::vec3 targetDirection(a_currentPosition - a_targetPosition);

	if (glm::length(targetDirection) > 0.0f)
	{
		targetDirection = glm::normalize(targetDirection);
	}

	// Calculate new velocity.
	glm::vec3 newVelocity = (targetDirection * m_fSpeed) - m_velocity;
	return newVelocity;
}

glm::vec3 BrainComponent::CalculateWanderForce(const glm::vec3& a_forwardDirection,
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
	return CalculateSeekForce(m_wanderPoint, a_currentPosition);
}