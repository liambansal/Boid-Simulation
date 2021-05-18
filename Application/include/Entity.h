//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef ENTITY_H
#define ENTITY_H

// Header includes.
#include "Component.h"
#include <map>
#include <string>
#include <vector>

// Forward declarations.
class Framework;
class Scene;

class Entity
{
public:
	Entity();
	Entity(Entity& a_rEntityCopy,
		Scene* a_pScene);
	~Entity();

	// Update entity's components one by one.
	virtual void Update(float a_deltaTime);
	// Draw entity's components one by one.
	virtual void Draw(Framework* a_pRenderingFramework);
	inline void AddComponent(COMPONENT_TYPE a_key, Component* a_pComponent);

	inline void SetTag(std::string a_newTag);

	inline Component* GetComponentOfType(COMPONENT_TYPE a_componentType) const;
	inline const unsigned int GetID() const;
	inline const std::string GetTag() const;

private:
	static unsigned int ms_uiEntityCount;
	unsigned int m_uiEntityID;
	std::string m_tag;
	std::map<COMPONENT_TYPE, Component*> m_components;
};

void Entity::AddComponent(COMPONENT_TYPE a_key, Component* a_pComponent)
{
	m_components.insert(std::pair<COMPONENT_TYPE, Component*>(a_key, a_pComponent));
}

void Entity::SetTag(std::string a_newTag)
{
	m_tag = a_newTag;
}

Component* Entity::GetComponentOfType(COMPONENT_TYPE a_componentType) const
{
	if (m_components.count(a_componentType) > 0)
	{
		return m_components.find(a_componentType)->second;
	}

	return nullptr;
}

const unsigned int Entity::GetID() const
{
	return m_uiEntityID;
}

const std::string Entity::GetTag() const
{
	return m_tag;
}

#endif // !ENTITY_H.
