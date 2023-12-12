//////////////////////////////
// Author: Liam Bansal
// Date Created: 30/05/2021
//////////////////////////////

// File's header.
#include "ColliderComponent.h"
#include "Entity.h"
#include "OctTree.h"
#include "TransformComponent.h"

bool ColliderComponent::m_bDrawColliders = false;

ColliderComponent::ColliderComponent(Entity* a_pOwner,
	const OctTree<Entity, glm::vec3>* a_pOctTree) : Component(a_pOwner),
	m_bIsColliding(false),
	m_fLastUpdate(0.0f),
	m_fColliderRange(0.5f),
	m_collisionColldiers(),
	mc_pOctTree(a_pOctTree) {
	TransformComponent* pTransform = static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
	m_boundary = Boundary<glm::vec3>(pTransform ? pTransform->GetPosition() : new glm::vec3(0.0f),
		glm::vec3(m_fColliderRange),
		false);
	m_componentType = COMPONENT_TYPE_COLLIDER;
}

ColliderComponent::ColliderComponent(Entity* a_pOwner,
	const OctTree<Entity, glm::vec3>* a_pOctTree,
	ColliderComponent& a_rColliderToCopy) : Component(a_pOwner),
	m_bIsColliding(false),
	m_fLastUpdate(0.0f),
	m_fColliderRange(a_rColliderToCopy.m_fColliderRange),
	m_collisionColldiers(),
	mc_pOctTree(a_pOctTree) {
	TransformComponent* pTransform = static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));
	m_boundary = Boundary<glm::vec3>(pTransform ? pTransform->GetPosition() : new glm::vec3(0.0f),
		a_rColliderToCopy.GetBoundary()->GetDimensions(),
		false);
	m_componentType = COMPONENT_TYPE_COLLIDER;
}

ColliderComponent::~ColliderComponent() {}

void ColliderComponent::Update(float a_fDeltaTime) {
	if (!Entity::GetCollisionsState()) {
		m_bIsColliding = false;
		return;
	}

	m_fLastUpdate += a_fDeltaTime;
	const float updateStep = 0.15f;

	if (m_fLastUpdate >= updateStep) {
		m_fLastUpdate = 0.0f;
		RegisterCollisions();
		UnregisterCollisions();
	}
}

void ColliderComponent::Draw(Framework* a_pRenderingFramework) {
	if (!m_bDrawColliders) {
		return;
	}

	m_boundary.Draw();
}

void ColliderComponent::RegisterCollisions() {
	if (!mc_pOctTree) {
		return;
	}

	Entity* pOwnerEntity = GetEntity();

	if (!pOwnerEntity) {
		return;
	}

	TransformComponent* pOwnerEntityTransform = static_cast<TransformComponent*>(pOwnerEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!pOwnerEntityTransform) {
		return;
	}

	// Volume of space to search through for possible collisions.
	Boundary<glm::vec3> queryZone(pOwnerEntityTransform->GetPosition(),
		glm::vec3(m_fColliderRange));
	// References to entities within the query zone.
	std::vector<Entity*> containedEntities;
	// Search the query zone for entities.
	mc_pOctTree->Query(queryZone, containedEntities);

	if (containedEntities.size() > 0) {
		for (Entity* pEntity : containedEntities) {
			// Make sure we're not examining this entity.
			if (GetEntity() == pEntity || pEntity->GetTag() == "Marker") {
				continue;
			}

			// The collider of the entity that has triggered a collision with this.
			ColliderComponent* otherEntityCollider = static_cast<ColliderComponent*>(pEntity->GetComponentOfType(COMPONENT_TYPE_COLLIDER));

			if (!otherEntityCollider) {
				continue;
			}

			if (m_boundary.Overlaps(otherEntityCollider->m_boundary)) {
				// Has a collision already been registered with the overlapping collider?
				bool collisionRegistered = false;

				for (ColliderComponent* registeredCollider : m_collisionColldiers) {
					if (registeredCollider == otherEntityCollider) {
						collisionRegistered = true;
						break;
					}
				}

				if (collisionRegistered) {
					// Don't register a collision more than once.
					continue;
				}

				m_collisionColldiers.push_back(otherEntityCollider);
				m_bIsColliding = true;
			}
		}
	}
}

// Unregisters collisions with colliders we're no longer overlapping.
void ColliderComponent::UnregisterCollisions() {
	for (std::vector<ColliderComponent*>::const_iterator iterator = m_collisionColldiers.cbegin();
		iterator != m_collisionColldiers.cend();) {
		const Entity* pOtherEntity = (*iterator)->GetEntity();

		if (!pOtherEntity) {
			continue;
		}

		const ColliderComponent* otherEntityCollider = static_cast<ColliderComponent*>(pOtherEntity->GetComponentOfType(COMPONENT_TYPE_COLLIDER));

		if (!otherEntityCollider) {
			continue;
		}

		// Find out if the collision has ended.
		if (!m_boundary.Overlaps(otherEntityCollider->m_boundary)) {
			// Erase to remove the collider as a registered collision.
			// Erase returns the element that's after the one being removed.
			iterator = m_collisionColldiers.erase(iterator);
		} else {
			++iterator;
		}
	}

	if (m_collisionColldiers.size() == 0) {
		m_bIsColliding = false;
	}
}
