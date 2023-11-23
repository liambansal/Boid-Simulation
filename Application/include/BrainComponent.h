//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

// Header dependencies.
#include "Component.h"
#include "glm/glm.hpp"
#include <vector>

// Forward declarations
class ColliderComponent;
class Entity;
class Framework;
class Scene;
class TransformComponent;

// Typedefs.
typedef std::vector<Entity*> pEntityVector;

/// <summary>
/// Allows a boid to automatically move itself around a scene.
/// </summary>
class BrainComponent : public Component {
public:
	BrainComponent(Entity* a_pOwner,
		Scene* a_pScene);
	BrainComponent(Entity* a_pOwner,
		BrainComponent& a_rBrainToCopy,
		Scene* a_pScene);
	~BrainComponent() {}

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Framework* a_pRenderingFramework) {}

	static inline void SetSeparationForce(float a_force);
	static inline void SetAlignmentForce(float a_force);
	static inline void SetCohesionForce(float a_force);
	static inline void SetWanderForce(float a_force);

	inline glm::vec3 GetVelocity() const;
	static inline float GetSeparationForce();
	static inline float GetAlignmentForce();
	static inline float GetCohesionForce();
	static inline float GetWanderForce();
	/// <summary>
	/// Gets a collection of all the nearby entities.
	/// </summary>
	/// <param name="a_currentPosition"> The entity's current world position. </param>
	/// <returns> A collection of entity pointers. </returns>
	pEntityVector GetNeighbouringEntities(glm::vec3 a_currentPosition) const;

private:
	/// <summary>
	/// Generates a new seek velocity for the boid.
	/// </summary>
	/// <param name="a_rTargetPosition"> The position to move towards. </param>
	/// <param name="a_rCurrentPosition"> The boid's current position. </param>
	/// <returns> The boid's new seek velocity. </returns>
	glm::vec3 CalculateSeekVelocity(const glm::vec3& a_rTargetPosition,
		const glm::vec3& a_rCurrentPosition) const;
	/// <summary>
	/// Generates a new wander velocity for the boid relative to its current forward direction.
	/// </summary>
	/// <param name="a_rForwardDirection"> The boid's forward direction. </param>
	/// <param name="a_rCurrentPosition"> The boid's current position. </param>
	/// <returns> The boid's new wander velocity. </returns>
	glm::vec3 CalculateWanderVelocity(const glm::vec3& a_rForwardDirection,
		const glm::vec3& a_rCurrentPosition) const;
	/// <summary>
	/// Generates a new separation velocity that moves the boid away from its neighbours.
	/// </summary>
	/// <param name="a_separationVelocity"> The boid's current separation velocity. </param>
	/// <param name="a_targetVector"> The amount to increase the boid's separation velocity by. </param>
	/// <param name="a_uiNeighbourCount"> The number of entities to move away from. </param>
	/// <returns> The boid's new separation velocity. </returns>
	glm::vec3 CalculateSeparationVelocity(glm::vec3 a_separationVelocity,
		glm::vec3 a_targetVector,
		unsigned int a_uiNeighbourCount) const;
	/// <summary>
	/// Generates a new alignment velocity that moves the boid along a similar direction to its neighbours.
	/// </summary>
	/// <param name="a_alignmentVelocity"> The boid's current alignment velocity. </param>
	/// <param name="a_targetVector"> The amount to increase the boid's alignment velocity by. </param>
	/// <returns> The boid's new alignment velocity. </returns>
	glm::vec3 CalculateAlignmentVelocity(glm::vec3 a_alignmentVelocity,
		glm::vec3 a_targetVector)const;
	/// <summary>
	/// Generates a new cohesion velocity that moves the boid towards its neighbours.
	/// </summary>
	/// <param name="a_cohesionVelocity"> The boid's current cohesion velocity. </param>
	/// <param name="a_cohesionChange"> The amount to change the boid's cohesion velocity by. </param>
	/// <param name="a_currentPosition"> The boid's current position. </param>
	/// <returns> The boid's new cohesion velocity. </returns>
	glm::vec3 CalculateCohesionVelocity(glm::vec3 a_cohesionVelocity,
		glm::vec3 a_cohesionChange,
		glm::vec3 a_currentPosition)const;
	/// <summary>
	/// Calculates the boid's overall movement.
	/// </summary>
	/// <param name="a_rEntityPosition"> The boid's current position. </param>
	/// <param name="a_rEntityForward"> The boid's forward direction. </param>
	glm::vec3 CalculateNewMovementVelocity(glm::vec3& a_rEntityPosition,
		glm::vec3& a_rEntityForward,
		pEntityVector nearbyEntities) const;
	/// <summary>
	/// Calculates a new velocity that moves the entity away from collisions.
	/// </summary>
	/// <param name="a_entityPosition"> The entity's current position. </param>
	void CalculateCollisionVelocity(const glm::vec3 a_entityPosition);

	/// <summary>
	/// Updates the entity's transform matrix with the entity's current position and rotation data.
	/// </summary>
	/// <param name="a_pTransform"> The entity's transform component. </param>
	/// <param name="a_pPosition"> The entity's position. </param>
	/// <param name="a_pForward"> The entity's forward direction. </param>
	void UpdateMatrix(TransformComponent* a_pTransform,
		glm::vec3* a_pPosition,
		glm::vec3* a_pForward);

	/// <summary>
	/// Gets a semi-random position near to the specified position.
	/// </summary>
	/// <param name="a_originPosition"> The point in space where the semi-random position in centred around. </param>
	/// <returns> A position. </returns>
	glm::vec3 GetRandomNearbyPoint(glm::vec3 a_originPosition) const;

	/// <summary>
	/// The strength of the force that pulls two or more boids apart.
	/// </summary>
	static float ms_fSeparationForce;
	/// <summary>
	/// The strength of the force that causes a boid to move in the same direction as its neighbours.
	/// </summary>
	static float ms_fAlignmentForce;
	/// <summary>
	/// The strength of the force that brings two or more boids together.
	/// </summary>
	static float ms_fCohesionForce;
	/// <summary>
	/// The strength of the force that causes a boid to move to a random point in space.
	/// </summary>
	static float ms_fWanderForce;
	/// <summary>
	/// The number of nearby boids.
	/// </summary>
	unsigned int m_uiNeighbourCount;
	const float mc_fSpeed;
	/// <summary>
	/// The limit for the boid's velocity components i.e. x, y, and z.
	/// </summary>
	const float mc_fMaximumVelocity;
	/// <summary>
	/// Any boid beyond this distance is not considered to be a neighbour.
	/// </summary>
	const float mc_fMaximumNeighbourDistance;
	/// <summary>
	/// The timestamp for when this component was last updated.
	/// Always starts at zero when the entity is created.
	/// </summary>
	float m_fLastUpdate;
	/// <summary>
	/// a velocity that describes the boids current movement.
	/// </summary>
	glm::vec3 m_currentMovementVelocity;
	/// <summary>
	/// The boids current movement direction as calculated by its behavioral forces.
	/// </summary>
	glm::vec3 m_newMovementVelocity;
	/// <summary>
	/// The separation velocity that's calculated based on collisions, to move a boid away from collisions.
	/// </summary>
	glm::vec3 m_collisionSeparationVelocity;
	/// <summary>
	/// The scene that the entity belongs to.
	/// </summary>
	Scene* m_pScene;
	ColliderComponent* m_pEntityCollider;
};

void BrainComponent::SetSeparationForce(float a_force) {
	ms_fSeparationForce = a_force;
}

void BrainComponent::SetAlignmentForce(float a_force) {
	ms_fAlignmentForce = a_force;
}

void BrainComponent::SetCohesionForce(float a_force) {
	ms_fCohesionForce = a_force;
}

void BrainComponent::SetWanderForce(float a_force) {
	ms_fWanderForce = a_force;
}

glm::vec3 BrainComponent::GetVelocity() const {
	return m_currentMovementVelocity;
}

float BrainComponent::GetSeparationForce() {
	return ms_fSeparationForce;
}

float BrainComponent::GetAlignmentForce() {
	return ms_fAlignmentForce;
}

float BrainComponent::GetCohesionForce() {
	return ms_fCohesionForce;
}

float BrainComponent::GetWanderForce() {
	return ms_fWanderForce;
}

#endif // !BRAIN_COMPONENT_H
