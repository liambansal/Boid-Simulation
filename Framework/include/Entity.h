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

	inline void AddComponent(Component* a_pComponent);
	// Returns the first found component attached to this entity of a 
	// specified type, if present.
	Component* GetComponentOfType(COMPONENT_TYPE a_componentType) const;
	inline const unsigned int GetID() const;

private:
	static unsigned int ms_uiEntityCount;
	unsigned int m_uiEntityID;

	static std::map<const unsigned int, Entity*> ms_EntityList;
	std::vector<Component*> m_components;
};

void Entity::AddComponent(Component* a_pComponent)
{
	m_components.push_back(a_pComponent);
}

inline const unsigned int Entity::GetID() const
{
	return m_uiEntityID;
}

#endif // !ENTITY_H.
