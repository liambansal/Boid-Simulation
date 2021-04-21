//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#include "BrainComponent.h" // File's header.
#include "Entity.h"
#include "LearnOpenGL/shader.h"
#include "TransformComponent.h"

BrainComponent::BrainComponent(Entity* a_pOwner) : Component(a_pOwner),
	m_velocity(0.0f)
{
	m_componentType = COMPONENT_TYPE_AI;
}

void BrainComponent::Update(float a_deltaTime)
{
	
}

void BrainComponent::Draw(Shader a_pShader)
{}

glm::vec3 BrainComponent::CalculateSeekForce(const glm::vec3& a_v3Target,
	const glm::vec3& a_v3CurrentPosition) const
{

}

glm::vec3 BrainComponent::CalculateFleeForce(const glm::vec3& a_v3Target,
	const glm::vec3& a_v3CurrentPosition) const
{

}

glm::vec3 BrainComponent::CalculateWanderForce(const glm::vec3& a_v3Forward,
	const glm::vec3& a_v3CurrentPosition) const
{

}