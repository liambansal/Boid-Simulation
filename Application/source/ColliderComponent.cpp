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
	mc_fColliderRange(0.5f),
	m_collisionColldiers(),
	mc_pOctTree(a_pOctTree)
{
	TransformComponent* pTransform = static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
	m_boundary = Boundary(pTransform ? pTransform->GetPosition() : new glm::vec3(0.0f),
		glm::vec3(mc_fColliderRange));
	m_componentType = COMPONENT_TYPE_COLLIDER;
}

ColliderComponent::ColliderComponent(Entity* a_pOwner,
	const OctTree<Entity, glm::vec4>* a_pOctTree,
	ColliderComponent& a_rColliderToCopy) : Component(a_pOwner),
	m_bIsColliding(false),
	m_fLastUpdate(0.0f),
	mc_fColliderRange(a_rColliderToCopy.mc_fColliderRange),
	m_collisionColldiers(),
	mc_pOctTree(a_pOctTree)
{
	TransformComponent* pTransform = static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
	m_boundary = Boundary(pTransform ? pTransform->GetPosition() : new glm::vec3(0.0f),
		a_rColliderToCopy.GetBoundary()->GetDimensions());
	m_componentType = COMPONENT_TYPE_COLLIDER;
}

ColliderComponent::~ColliderComponent()
{}

void ColliderComponent::Update(float a_fDeltaTime)
{
	static int frames = 0;
	m_fLastUpdate += a_fDeltaTime;
	const float updateStep = 0.5f;

	if (m_fLastUpdate >= updateStep)
	{
		m_fLastUpdate = 0.0f;
		RegisterCollisions();
	}

	UnregisterCollisions();
}

void ColliderComponent::Draw(Framework* a_pRenderingFramework)
{}

void ColliderComponent::RegisterCollisions()
{
	if (!mc_pOctTree)
	{
		return;
	}

	Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity)
	{
		return;
	}

	TransformComponent* pOwnerEntityTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pOwnerEntityTransform)
	{
		return;
	}

	// Volume of space to search through for possible collisions.
	Boundary queryZone(pOwnerEntityTransform->GetPosition(),
		glm::vec3(mc_fColliderRange));
	// References to entities within the query zone.
	std::vector<Entity*> containedEntities;
	// Search the query zone for entities.
	mc_pOctTree->Query(queryZone, containedEntities);

	if (containedEntities.size() > 0)
	{
		for (Entity* pEntity : containedEntities)
		{
			// Make sure we're not examining this entity.
			if (GetEntity() == pEntity || pEntity->GetTag() == "Marker")
			{
				continue;
			}

			ColliderComponent* otherEntityCollider = static_cast<ColliderComponent*>(pEntity->GetComponentOfType(COMPONENT_TYPE_COLLIDER));

			if (!otherEntityCollider)
			{
				continue;
			}

			bool collisionRegistered = false;

			for (ColliderComponent* registeredCollider : m_collisionColldiers)
			{
				if (registeredCollider == otherEntityCollider)
				{
					// Don't register a collision more than once.
					collisionRegistered = true;
					break;
				}
			}

			if (collisionRegistered)
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

void ColliderComponent::UnregisterCollisions()
{
	for (std::vector<ColliderComponent*>::const_iterator iterator = m_collisionColldiers.cbegin();
		iterator != m_collisionColldiers.cend();)
	{
		const Entity* pOtherEntity = (*iterator)->GetEntity();

		if (!pOtherEntity)
		{
			continue;
		}

		const ColliderComponent* otherEntityCollider = static_cast<ColliderComponent*>(pOtherEntity->GetComponentOfType(COMPONENT_TYPE_COLLIDER));

		if (!otherEntityCollider)
		{
			continue;
		}

		// Find out when the collision has ended.
		if (!m_boundary.Overlaps(otherEntityCollider->m_boundary))
		{
			// Erase returns the element that's after the one being removed.
			iterator = m_collisionColldiers.erase(iterator);
		}
		else
		{
			++iterator;
		}
	}

	if (m_collisionColldiers.size() == 0)
	{
		m_bIsColliding = false;
	}
}
