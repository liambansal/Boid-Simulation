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
template <typename TObject, typename TPosition>
class OctTree;

class ColliderComponent : public Component
{
public:
	ColliderComponent(Entity* a_pOwner,
		const OctTree<Entity, glm::vec4>* a_pOctTree);
	ColliderComponent(Entity* a_pOwner,
		const OctTree<Entity, glm::vec4>* a_pOctTree,
		ColliderComponent& a_rColliderToCopy);
	~ColliderComponent()
	{}

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Framework* a_pRenderingFramework)
	{}

	inline const bool IsColliding() const;
	
	inline const Boundary* GetBoundary() const;
	inline const std::vector<ColliderComponent*> GetCollisions() const;

private:
	void RegisterCollisions();

	bool m_bIsColliding;
	float m_fLastUpdate;
	Boundary m_boundary;
	std::vector<ColliderComponent*> m_collisionColldiers;
	const OctTree<Entity, glm::vec4>* mc_pOctTree;
};

const bool ColliderComponent::IsColliding() const
{
	return m_bIsColliding;
}

const Boundary* ColliderComponent::GetBoundary() const
{
	return &m_boundary;
}

const std::vector<ColliderComponent*> ColliderComponent::GetCollisions() const
{
	return m_collisionColldiers;
}

#endif // !COLLIDER_COMPONENT_H
