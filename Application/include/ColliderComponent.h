//////////////////////////////
// Author: Liam Bansal
// Date Created: 30/05/2021
//////////////////////////////

#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

// Header includes.
#include "Component.h"
#include "Boundary.h"
#include "glm/glm.hpp"
#include <vector>

// Forward declarations.
class Entity;
template <typename TObject, typename TVector>
class OctTree;

// Creates collisions within a volume of space.
class ColliderComponent : public Component
{
public:
	ColliderComponent(Entity* a_pOwner,
		const OctTree<Entity, glm::vec3>* a_pOctTree);
	ColliderComponent(Entity* a_pOwner,
		const OctTree<Entity, glm::vec3>* a_pOctTree,
		ColliderComponent& a_rColliderToCopy);
	~ColliderComponent();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Framework* a_pRenderingFramework);

	// Returns true if there is a registered collision.
	inline const bool IsColliding() const;
	
	// Set dimensions for the volume of space the collider will occupy.
	inline void SetDimensions(float a_dimensionScalar);

	inline Boundary<glm::vec3>* GetBoundary();
	// Returns a vector of all the colliders that we've registered a collision with.
	inline const std::vector<ColliderComponent*>& GetCollisions() const;

private:
	// Registres collisions with colliders we're overlapping.
	void RegisterCollisions();
	// Unregisters collisions with colliders we're no longer overlapping.
	void UnregisterCollisions();

	bool m_bIsColliding;
	float m_fLastUpdate;
	float m_fColliderRange;
	// Position and dimensions of the collider's bounds.
	Boundary<glm::vec3> m_boundary;
	// All the colliders that this is colliding with.
	std::vector<ColliderComponent*> m_collisionColldiers;
	const OctTree<Entity, glm::vec3>* mc_pOctTree;
};

// Returns true if there is a registered collision.
const bool ColliderComponent::IsColliding() const
{
	return m_bIsColliding;
}

// Set dimensions for the volume of space the collider will occupy.
void ColliderComponent::SetDimensions(float a_dimensionScalar)
{
	m_fColliderRange = a_dimensionScalar;
	m_boundary.SetDimensions(glm::vec3(m_fColliderRange));
}

Boundary<glm::vec3>* ColliderComponent::GetBoundary()
{
	return &m_boundary;
}

// Returns a vector of all the colliders that we've registered a collision with.
const std::vector<ColliderComponent*>& ColliderComponent::GetCollisions() const
{
	return m_collisionColldiers;
}

#endif // !COLLIDER_COMPONENT_H
