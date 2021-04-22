//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

#include "Component.h"
#include "glm/glm.hpp"

class Entity;

class BrainComponent : public Component
{
public:
	BrainComponent(Entity* a_pOwner);

	virtual void Update(float a_deltaTime);
	virtual void Draw(Shader* a_pShader);

	inline glm::vec3 GetVelocity() const;

private:
	// Steering behaviours
	glm::vec3 CalculateSeekForce(const glm::vec3& a_targetPosition,
		const glm::vec3& a_currentPosition) const;
	glm::vec3 CalculateFleeForce(const glm::vec3& a_targetPosition,
		const glm::vec3& a_currentPosition) const;
	glm::vec3 CalculateWanderForce(const glm::vec3& a_forwardDirection,
		const glm::vec3& a_currentPosition);

	// Flocking behaviours
	glm::vec3 CalculateSeparationForce();
	glm::vec3 CalculateAlignmentForce();
	glm::vec3 CalculateCohesionForce();

	const float m_fSpeed;
	glm::vec3 m_velocity;
	glm::vec3 m_wanderPoint;
};

glm::vec3 BrainComponent::GetVelocity() const
{
	return m_velocity;
}

#endif // !BRAIN_COMPONENT_H
