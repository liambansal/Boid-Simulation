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

/// <summary>
/// The base class from which all entity types should derive from to support the creation of a 'game object'.
/// </summary>
class Entity
{
public:
	Entity();
	/// <summary>
	/// Copy constructor for creating a new entity from an existing instance.
	/// </summary>
	/// <param name="a_rEntityCopy"> The entity instance to copy. </param>
	/// <param name="a_pScene"> The scene in which the entity to copy is a part of. </param>
	Entity(Entity& a_rEntityCopy, Scene* a_pScene);
	~Entity();

	/// <summary>
	/// Updates the entity and its components.
	/// Always and only call once per frame.
	/// </summary>
	/// <param name="a_deltaTime"></param>
	virtual void Update(float a_deltaTime);
	/// <summary>
	/// Draws the entity on-screen.
	/// </summary>
	/// <param name="a_pRenderingFramework"> The custom program that handles drawing the entity. </param>
	virtual void Draw(Framework* a_pRenderingFramework);
	/// <summary>
	/// Attaches a new component to the entity.
	/// </summary>
	/// <param name="a_key"> The component's type. </param>
	/// <param name="a_pComponent"> A pointer to the component instance to attach to the entity. </param>
	inline void AddComponent(COMPONENT_TYPE a_key, Component* a_pComponent);

	inline void SetTag(std::string a_newTag);

	inline Component* GetComponentOfType(COMPONENT_TYPE a_componentType) const;
	inline const unsigned int GetID() const;
	inline const std::string GetTag() const;

private:
	/// <summary>
	/// The number of entities that currently exist across all scenes.
	/// </summary>
	static unsigned int ms_uiEntityCount;
	/// <summary>
	/// A unique identifier for each entity.
	/// </summary>
	unsigned int m_uiEntityID;
	/// <summary>
	/// A string identifier that's usually used for finding an entity.
	/// </summary>
	std::string m_tag;
	/// <summary>
	/// A collection of all the components that are currently attached to the entity.
	/// </summary>
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
