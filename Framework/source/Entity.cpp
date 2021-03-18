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
std::map<const unsigned int, Entity*> Entity::ms_EntityList;

Entity::Entity()
{
	// Assign and increment entity ID.
	m_uiEntityID = ms_uiEntityCount++;
	// Add this entity to our list.
	ms_EntityList.insert(EntityPair(m_uiEntityID, this));
}

Entity::~Entity()
{
	ComponentList::iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator < m_components.end();
		++componentIterator)
	{
		if (*componentIterator)
		{
			delete *componentIterator;
			*componentIterator = nullptr;
		}
	}
}

// Update entity's components one by one.
void Entity::Update(float a_deltaTime)
{
	ComponentList::iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator < m_components.end();
		++componentIterator)
	{
		Component* pComponent = *componentIterator;

		if (pComponent)
		{
			pComponent->Update(a_deltaTime);
		}
	}
}

// Draw entity's components one by one.
void Entity::Draw(Shader* a_pShader)
{
	ComponentList::iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator < m_components.end();
		++componentIterator)
	{
		Component* pComponent = *componentIterator;

		if (pComponent)
		{
			pComponent->Draw(a_pShader);
		}
	}
}

// Returns the first found component attached to this entity of a specified 
// type, if present.
Component* Entity::GetComponentOfType(COMPONENT_TYPE a_componentType) const
{
	ComponentList::const_iterator componentIterator;

	for (componentIterator = m_components.begin();
		componentIterator < m_components.end();
		++componentIterator)
	{
		Component* pComponent = *componentIterator;

		if (pComponent && pComponent->GetComponentType() == a_componentType)
		{
			return pComponent;
		}
	}

	return nullptr;
}
