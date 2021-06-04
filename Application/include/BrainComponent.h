//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

// Header dependencies.
#include "Component.h"
#include "glm/glm.hpp"

// Forward declarations
class ColliderComponent;
class Entity;
class Framework;
class Scene;
class TransformComponent;

// Provides the default behaviour for a boid entity.
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

	virtual void Update(float a_fDeltaTime);
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
	// Calculates a point to move towards.
	glm::vec3 CalculateSeekVelocity(const glm::vec3& a_rTargetPosition,
		const glm::vec3& a_rCurrentPosition) const;
	// Calculates a point to move away from.
	glm::vec3 CalculateFleeVelocity(const glm::vec3& a_rTargetPosition,
		const glm::vec3& a_rCurrentPosition) const;
	// Calculates a semi-random point to move towards.
	glm::vec3 CalculateWanderVelocity(const glm::vec3& a_rForwardDirection,
		const glm::vec3& a_rCurrentPosition);

	// Calculate velocity to move boids away from each other.
	glm::vec3 CalculateSeparationVelocity(glm::vec3 a_separationVelocity,
		glm::vec3 a_targetVector,
		unsigned int a_uiNeighbourCount);
	// Calculate velocity to move boids alongside each other.
	glm::vec3 CalculateAlignmentVelocity(glm::vec3 a_alignmentVelocity,
		glm::vec3 a_targetVector);
	// Calculate velocity to move boids towards each other.
	glm::vec3 CalculateCohesionVelocity(glm::vec3 a_cohesionVelocity,
		glm::vec3 a_targetPosition,
		glm::vec3 a_localPosition);
	// Calculates the boids flocking behaviours.
	void CalculateBehaviouralVelocity(glm::vec3& a_rEntityPosition,
		glm::vec3& a_rEntityForward);

	// Calculates the separation velocity produced by collisions.
	void CalculateCollisionVelocity(const glm::vec3 a_entityPosition);
	void UpdateMatrix(TransformComponent* a_pTransform,
		glm::vec3* a_pPosition,
		glm::vec3* a_pForward);
	// Gets a semi-random position, based around the argument position.
	glm::vec3 GetRandomNearbyPoint(glm::vec3 a_originPosition) const;

	static float ms_fSeparationForce;
	static float ms_fAlignmentForce;
	static float ms_fCohesionForce;
	static float ms_fWanderForce;

	unsigned int m_uiNeighbourCount;
	const float mc_fSpeed;
	const float mc_fMaximumVelocity;
	const float mc_fMaximumNeighbourDistance;
	float m_fLastUpdate;
	glm::vec3 m_currentVelocity;
	// Velocity calculated by behavioral forces.
	glm::vec3 m_behavioralVelocity;
	// separation velocity calculated based collisions.
	glm::vec3 m_collisionSeparationVelocity;
	glm::vec3 m_wanderPoint;
	Scene* m_pScene;
	ColliderComponent* m_pEntityCollider;
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
	return m_currentVelocity;
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
