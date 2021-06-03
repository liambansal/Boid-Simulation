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

	inline const bool IsColliding() const;
	
	inline void SetDimensions(float a_dimensionScalar);

	inline Boundary<glm::vec3>* GetBoundary();
	inline const std::vector<ColliderComponent*>& GetCollisions() const;

private:
	void RegisterCollisions();
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

const bool ColliderComponent::IsColliding() const
{
	return m_bIsColliding;
}

void ColliderComponent::SetDimensions(float a_dimensionScalar)
{
	m_fColliderRange = a_dimensionScalar;
	m_boundary.SetDimensions(glm::vec3(m_fColliderRange));
}

Boundary<glm::vec3>* ColliderComponent::GetBoundary()
{
	return &m_boundary;
}

const std::vector<ColliderComponent*>& ColliderComponent::GetCollisions() const
{
	return m_collisionColldiers;
}

#endif // !COLLIDER_COMPONENT_H
