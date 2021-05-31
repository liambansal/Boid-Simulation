//////////////////////////////
// Author: Liam Bansal
// Date Created: 30/05/2021
//////////////////////////////

// File's header.
#include "ColliderComponent.h"
#include "Entity.h"
#include "OctTree.h"
#include "TransformComponent.h"
#include <vector>

ColliderComponent::ColliderComponent(Entity* a_pOwner,
	const OctTree<Entity, glm::vec4>* a_pOctTree) : Component(a_pOwner),
	m_bIsColliding(false),
	m_fLastUpdate(0.0f),
	m_collisionColldiers(),
	mc_pOctTree(a_pOctTree)
{
	TransformComponent* pTransform = static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
	const float colliderRange = 1.0f;
	m_boundary = Boundary(pTransform ? pTransform->GetPosition() : new glm::vec3(0.0f),
		glm::vec3(colliderRange));
	m_componentType = COMPONENT_TYPE_COLLIDER;
}

ColliderComponent::ColliderComponent(Entity* a_pOwner,
	const OctTree<Entity, glm::vec4>* a_pOctTree,
	ColliderComponent& a_rColliderToCopy) : Component(a_pOwner),
	m_bIsColliding(false),
	m_fLastUpdate(0.0f),
	m_collisionColldiers(),
	mc_pOctTree(a_pOctTree)
{
	TransformComponent* pTransform = static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
	m_boundary = Boundary(pTransform ? pTransform->GetPosition() : new glm::vec3(0.0f),
		a_rColliderToCopy.GetBoundary()->GetDimensions());
	m_componentType = COMPONENT_TYPE_COLLIDER;
}

void ColliderComponent::Update(float a_fDeltaTime)
{
	m_fLastUpdate += a_fDeltaTime;
	const float updateStep = 5.0f;

	//if (m_fLastUpdate >= updateStep)
	//{
		m_fLastUpdate = 0.0f;
		RegisterCollisions();
	//}
}

void ColliderComponent::RegisterCollisions()
{
	// Clear all collider references before each update.
	m_collisionColldiers.clear();
	m_bIsColliding = false;

	if (!mc_pOctTree)
	{
		return;
	}

	// TODO: null check pointers.
	// Volume of space to search through for nearby entities.
	Boundary queryZone(static_cast<TransformComponent*>(GetEntity()->GetComponentOfType(COMPONENT_TYPE_TRANSFORM))->GetPosition(),
		glm::vec3(1.25f));
	// References to entities within the query zone.
	std::vector<Entity*> containedEntities;
	// Search the query zone for entities.
	mc_pOctTree->Query(queryZone, containedEntities);

	if (containedEntities.size() > 0)
	{
		for (Entity* pEntity : containedEntities)
		{
			// Make sure we're not examining this entity.
			if (GetEntity() == pEntity)
			{
				continue;
			}

			ColliderComponent* otherEntityCollider = static_cast<ColliderComponent*>(pEntity->GetComponentOfType(COMPONENT_TYPE_COLLIDER));

			if (!otherEntityCollider)
			{
				continue;
			}

			if (m_boundary.Overlaps(otherEntityCollider->m_boundary))
			{
				m_collisionColldiers.push_back(otherEntityCollider);
				m_bIsColliding = true;
			}
		}
	}
}
