//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "Entity.h"

// Typedefs.
typedef std::pair<const unsigned int, Entity*> EntityPair;
typedef std::vector<Component*> ComponentList;

// Static variables.
unsigned int Entity::ms_uiEntityCount = 0;
std::map<const unsigned int, Entity*> Entity::ms_EntityMap;

Entity::Entity()
{
	// Assign and increment entity ID.
	m_uiEntityID = ms_uiEntityCount++;
	// Add this entity to our list.
	ms_EntityMap.insert(EntityPair(m_uiEntityID, this));
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

