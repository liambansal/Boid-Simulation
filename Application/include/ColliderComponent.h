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

/// <summary>
/// Allows an entity to trigger collisions with other colliders.
/// </summary>
class ColliderComponent : public Component {
public:
	ColliderComponent(Entity* a_pOwner,
		const OctTree<Entity, glm::vec3>* a_pOctTree);
	ColliderComponent(Entity* a_pOwner,
		const OctTree<Entity, glm::vec3>* a_pOctTree,
		ColliderComponent& a_rColliderToCopy);
	~ColliderComponent();

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Framework* a_pRenderingFramework);

	/// <summary>
	/// Checks if the entity is in contact with another collider.
	/// </summary>
	/// <returns> True if the entity is colliding with something. </returns>
	inline const bool IsColliding() const;

	/// <summary>
	/// Sets the collider's width, height, and depth.
	/// </summary>
	/// <param name="a_dimensionScalar"> The value to set for each dimension. </param>
	inline void SetDimensions(float a_dimensionScalar);
	inline static void SetColliderDrawState(bool a_drawColliders);

	inline Boundary<glm::vec3>* GetBoundary();
	/// <summary>
	/// Returns a collection of all the entities that are in contact with this entity.
	/// </summary>
	/// <returns> All the colliding entities. </returns>
	inline const std::vector<ColliderComponent*>& GetCollisions() const;
	inline static const bool GetColliderDrawState() { return m_bDrawColliders; }

private:
	/// <summary>
	/// Detects if any new collisions have started with another entity.
	/// </summary>
	void RegisterCollisions();
	/// <summary>
	/// Removes all references to entities that are no longer colliding with this entity.
	/// </summary>
	void UnregisterCollisions();

	/// <summary>
	/// True if there's a collision between this entity and at least one other entity.
	/// </summary>
	bool m_bIsColliding;
	/// <summary>
	/// The timestamp for when this component was last updated.
	/// Always starts at zero when the entity is created.
	/// </summary>
	float m_fLastUpdate;
	/// <summary>
	/// The greatest distance between any two bounds of the collider.
	/// </summary>
	float m_fColliderRange;
	/// <summary>
	/// Stores the collider's position and dimensions.
	/// </summary>
	Boundary<glm::vec3> m_boundary;
	static bool m_bDrawColliders;
	/// <summary>
	/// A collection of all the colliders that are in contact with this entity.
	/// </summary>
	std::vector<ColliderComponent*> m_collisionColldiers;
	/// <summary>
	/// A pointer to the scenes oct-tree.
	/// </summary>
	const OctTree<Entity, glm::vec3>* mc_pOctTree;
};

const bool ColliderComponent::IsColliding() const {
	return m_bIsColliding;
}

void ColliderComponent::SetDimensions(float a_dimensionScalar) {
	m_fColliderRange = a_dimensionScalar;
	m_boundary.SetDimensions(glm::vec3(m_fColliderRange));
}

void ColliderComponent::SetColliderDrawState(bool a_drawColliders) {
	m_bDrawColliders = a_drawColliders;
}

Boundary<glm::vec3>* ColliderComponent::GetBoundary() {
	return &m_boundary;
}

const std::vector<ColliderComponent*>& ColliderComponent::GetCollisions() const {
	return m_collisionColldiers;
}

#endif // !COLLIDER_COMPONENT_H
