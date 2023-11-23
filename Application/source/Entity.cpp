//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "Entity.h"
#include "BrainComponent.h"
#include "ColliderComponent.h"
#include "ModelComponent.h"
#include "OctTree.h"
#include "Scene.h"
#include "TransformComponent.h"

// Typedefs.
typedef std::pair<const unsigned int, Entity*> EntityPair;
typedef std::pair<COMPONENT_TYPE, Component*> ComponentPair;
typedef std::vector<Component*> ComponentList;

// Static variable initializations.
unsigned int Entity::ms_uiEntityCount = 0;
bool Entity::ms_bCollisionsOn = true;

// Increment the entity count with each new instance.
Entity::Entity() : m_uiEntityID(ms_uiEntityCount++),
m_tag("") {}

Entity::Entity(Entity& a_rEntityCopy,
	Scene* a_pScene) {
	m_uiEntityID = ms_uiEntityCount++;
	m_tag = a_rEntityCopy.m_tag;

	// Go through each component.
	for (ComponentPair component : a_rEntityCopy.m_components) {
		COMPONENT_TYPE componentType = component.second->GetComponentType();

		// Find the component type.
		if (componentType == COMPONENT_TYPE_TRANSFORM) {
			TransformComponent* pTransformComponent = new TransformComponent(this,
				*static_cast<TransformComponent*>(a_rEntityCopy.GetComponentOfType(componentType)));
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pTransformComponent)));
		} else if (componentType == COMPONENT_TYPE_MODEL) {
			ModelComponent* pModelComponent = new ModelComponent(this,
				*static_cast<ModelComponent*>(a_rEntityCopy.GetComponentOfType(componentType)));
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pModelComponent)));
		} else if (componentType == COMPONENT_TYPE_BRAIN) {
			BrainComponent* pBrainComponent = new BrainComponent(this,
				*static_cast<BrainComponent*>(a_rEntityCopy.GetComponentOfType(componentType)),
				a_pScene);
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pBrainComponent)));
		} else if (componentType == COMPONENT_TYPE_COLLIDER) {
			ColliderComponent* pColliderComponent = new ColliderComponent(this,
				&a_pScene->GetOctTree(),
				*static_cast<ColliderComponent*>(a_rEntityCopy.GetComponentOfType(componentType)));
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pColliderComponent)));
		}
	}
}

Entity::~Entity() {
	m_components.clear();
}

void Entity::Update(float a_fDeltaTime) {
	std::map<COMPONENT_TYPE, Component*>::iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator != m_components.end();
		++componentIterator) {
		Component* pComponent = componentIterator->second;

		if (pComponent) {
			pComponent->Update(a_fDeltaTime);
		}
	}
}

void Entity::Draw(Framework* a_pRenderingFramework) {
	std::map<COMPONENT_TYPE, Component*>::iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator != m_components.end();
		++componentIterator) {
		Component* pComponent = componentIterator->second;

		if (pComponent) {
			pComponent->Draw(a_pRenderingFramework);
		}
	}
}

void Entity::SetCollisionsState(bool a_bCollisionsOn) {
	ms_bCollisionsOn = a_bCollisionsOn;
}
