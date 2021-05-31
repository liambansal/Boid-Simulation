//////////////////////////////
// Author: Liam Bansal
// Date Created: 30/05/2021
//////////////////////////////

// File's header.
#include "ColliderComponent.h"
#include "Entity.h"
#include "TransformComponent.h"


ColliderComponent::ColliderComponent(Entity* a_pOwner) : Component(a_pOwner),
	m_collider(static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM)) ?
		static_cast<TransformComponent*>(a_pOwner->GetComponentOfType(COMPONENT_TYPE_TRANSFORM))->GetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR) : glm::vec3(0.0f),
		glm::vec3(1.0f))
{}

ColliderComponent::ColliderComponent(Entity* a_pOwner,
	ColliderComponent& a_rColliderToCopy) : Component(a_pOwner),
	m_collider(a_rColliderToCopy.m_collider)
{}