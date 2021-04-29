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
	glm::vec3 CalculateSeekVelocity(const glm::vec3& a_rTargetPosition,
		const glm::vec3& a_rCurrentPosition) const;
	glm::vec3 CalculateFleeVelocity(const glm::vec3& a_rTargetPosition,
		const glm::vec3& a_rCurrentPosition) const;
	glm::vec3 CalculateWanderVelocity(const glm::vec3& a_rForwardDirection,
		const glm::vec3& a_rCurrentPosition);

	// Flocking behaviours
	glm::vec3 CalculateSeparationVelocity(glm::vec3 a_separationVelocity,
		glm::vec3 a_targetVector);
	glm::vec3 CalculateAlignmentVelocity(glm::vec3 a_alignmentVelocity,
		glm::vec3 a_targetVector);
	glm::vec3 CalculateCohesionVelocity(glm::vec3 a_cohesionVelocity,
		glm::vec3 a_targetVector,
		glm::vec3 a_localPosition);
	void CalculateBehaviouralVelocities(glm::vec3& a_rSeparationVelocity,
		glm::vec3& a_rAlignmentVelocity,
		glm::vec3& a_rCohesionVelocity);

	int m_iNeighbourCount;
	const float mc_fSpeed;
	const float mc_fMaximumVelocity;
	float m_fLastUpdate;
	glm::vec3 m_velocity;
	glm::vec3 m_wanderPoint;
};

glm::vec3 BrainComponent::GetVelocity() const
{
	return m_velocity;
}

#endif // !BRAIN_COMPONENT_H
