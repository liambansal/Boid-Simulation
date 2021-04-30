//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef ENTITY_H
#define ENTITY_H

// Header includes.
#include "Component.h"
#include <map>
#include <vector>

// Forward declarations.
class Shader;

class Entity
{
public:
	Entity();
	~Entity();

	// Update entity's components one by one.
	virtual void Update(float a_deltaTime);
	// Draw entity's components one by one.
	virtual void Draw(Shader* a_pShader);

	inline void AddComponent(COMPONENT_TYPE a_key, Component* a_pComponent);
	inline Component* GetComponentOfType(COMPONENT_TYPE a_componentType) const;
	inline const unsigned int GetID() const;
	inline static const std::map<const unsigned int, Entity*> GetEntityMap();

private:
	static unsigned int ms_uiEntityCount;
	unsigned int m_uiEntityID;

	static std::map<const unsigned int, Entity*> ms_EntityMap;
	std::map<COMPONENT_TYPE, Component*> m_components;
};

void Entity::AddComponent(COMPONENT_TYPE a_key, Component* a_pComponent)
{
	m_components.insert(std::pair<COMPONENT_TYPE, Component*>(a_key, a_pComponent));
}

Component* Entity::GetComponentOfType(COMPONENT_TYPE a_componentType) const
{
	return m_components.find(a_componentType)->second;
}

const unsigned int Entity::GetID() const
{
	return m_uiEntityID;
}

const std::map<const unsigned int, Entity*> Entity::GetEntityMap()
{
	return ms_EntityMap;
}

#endif // !ENTITY_H.
