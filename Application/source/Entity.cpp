//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "Entity.h"
#include "BrainComponent.h"
#include "ColliderComponent.h"
#include "ModelComponent.h"
#include "TransformComponent.h"

// Typedefs.
typedef std::pair<const unsigned int, Entity*> EntityPair;
typedef std::vector<Component*> ComponentList;

// Static variables.
unsigned int Entity::ms_uiEntityCount = 0;

// Assign and increment entity ID.
Entity::Entity() : m_uiEntityID(ms_uiEntityCount++),
	m_tag("")
{}

Entity::Entity(Entity& a_rEntityCopy,
	Scene* a_pScene)
{
	m_uiEntityID = ms_uiEntityCount++;
	m_tag = a_rEntityCopy.m_tag;

	for (auto iterator = a_rEntityCopy.m_components.begin(); iterator != a_rEntityCopy.m_components.end(); ++iterator)
	{
		COMPONENT_TYPE componentType = iterator->second->GetComponentType();

		if (componentType == COMPONENT_TYPE_TRANSFORM)
		{
			TransformComponent* pTransformComponent = new TransformComponent(this,
				*static_cast<TransformComponent*>(a_rEntityCopy.GetComponentOfType(componentType)));
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pTransformComponent)));
		}
		else if (componentType == COMPONENT_TYPE_MODEL)
		{
			ModelComponent* pModelComponent = new ModelComponent(this,
				*static_cast<ModelComponent*>(a_rEntityCopy.GetComponentOfType(componentType)));
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pModelComponent)));
		}
		else if (componentType == COMPONENT_TYPE_BRAIN)
		{
			BrainComponent* pBrainComponent = new BrainComponent(this,
				*static_cast<BrainComponent*>(a_rEntityCopy.GetComponentOfType(componentType)),
				a_pScene);
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pBrainComponent)));
		}
		else if (componentType == COMPONENT_TYPE_COLLIDER)
		{
			ColliderComponent* pColliderComponent = new ColliderComponent(this,
				*static_cast<ColliderComponent*>(a_rEntityCopy.GetComponentOfType(componentType)));
			m_components.insert(std::pair<COMPONENT_TYPE, Component*>(componentType,
				static_cast<Component*>(pColliderComponent)));
		}
	}
}

Entity::~Entity()
{
	m_components.clear();
}

// Update entity's components one by one.
void Entity::Update(float a_deltaTime)
{
	std::map<COMPONENT_TYPE, Component*>::iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator != m_components.end();
		++componentIterator)
	{
		Component* pComponent = componentIterator->second;

		if (pComponent)
		{
			pComponent->Update(a_deltaTime);
		}
	}
}

// Draw entity's components one by one.
void Entity::Draw(Framework* a_pRenderingFramework)
{
	std::map<COMPONENT_TYPE, Component*>::iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator != m_components.end();
		++componentIterator)
	{
		Component* pComponent = componentIterator->second;

		if (pComponent)
		{
			pComponent->Draw(a_pRenderingFramework);
		}
	}
}

