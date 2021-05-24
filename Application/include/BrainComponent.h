//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

#include "Component.h"
#include "glm/glm.hpp"

class Entity;
class Framework;
class Scene;

class BrainComponent : public Component
{
public:
	BrainComponent(Entity* a_pOwner,
		Scene* a_pScene);
	BrainComponent(Entity* a_pOwner,
		BrainComponent& a_rBrainToCopy,
		Scene* a_pScene);
	~BrainComponent()
	{}

	virtual void Update(float a_deltaTime);
	virtual void Draw(Framework* a_pRenderingFramework)
	{}

	static inline void SetSeparationForce(float a_force);
	static inline void SetAlignmentForce(float a_force);
	static inline void SetCohesionForce(float a_force);
	static inline void SetWanderForce(float a_force);

	inline glm::vec3 GetVelocity() const;
	static inline float GetSeparationForce();
	static inline float GetAlignmentForce();
	static inline float GetCohesionForce();
	static inline float GetWanderForce();

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
		glm::vec3 a_targetPosition,
		glm::vec3 a_localPosition);
	void CalculateBehaviouralVelocities(glm::vec3& a_rSeparationVelocity,
		glm::vec3& a_rAlignmentVelocity,
		glm::vec3& a_rCohesionVelocity);

	static float ms_fSeparationForce;
	static float ms_fAlignmentForce;
	static float ms_fCohesionForce;
	static float ms_fWanderForce;

	unsigned int m_uiNeighbourCount;
	const float mc_fSpeed;
	const float mc_fMaximumVelocity;
	const float mc_fMaximumNeighbourDistance;
	float m_fLastUpdate;
	glm::vec3 m_velocity;
	glm::vec3 m_wanderPoint;
	Scene* m_pScene;
};

void BrainComponent::SetSeparationForce(float a_force)
{
	ms_fSeparationForce = a_force;
}

void BrainComponent::SetAlignmentForce(float a_force)
{
	ms_fAlignmentForce = a_force;
}

void BrainComponent::SetCohesionForce(float a_force)
{
	ms_fCohesionForce = a_force;
}

void BrainComponent::SetWanderForce(float a_force)
{
	ms_fWanderForce = a_force;
}

glm::vec3 BrainComponent::GetVelocity() const
{
	return m_velocity;
}

float BrainComponent::GetSeparationForce()
{
	return ms_fSeparationForce;
}

float BrainComponent::GetAlignmentForce()
{
	return ms_fAlignmentForce;
}

float BrainComponent::GetCohesionForce()
{
	return ms_fCohesionForce;
}

float BrainComponent::GetWanderForce()
{
	return ms_fWanderForce;
}

#endif // !BRAIN_COMPONENT_H
