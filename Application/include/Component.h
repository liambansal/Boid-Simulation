//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef COMPONENT_H
#define COMPONENT_H

// Forwards declarations.
class Entity;
class Framework;

// An enum of all the possible component types.
enum COMPONENT_TYPE
{
	COMPONENT_TYPE_NONE,
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_MODEL,
	COMPONENT_TYPE_COLLIDER,
	COMPONENT_TYPE_BRAIN,
	COMPONENT_TYPE_COUNT
};

/// <summary>
/// The base class for all component classes used by entities.
/// Provides a set of generic functions and properties that are used throughout all the component classes.
/// </summary>
class Component
{
public:
	Component(Entity* a_pOwner);

	/// <summary>
	/// The traditional function for updating a component once per frame.
	/// </summary>
	/// <param name="a_deltaTime"> The amount of time that has passed between this frame and the last one. </param>
	virtual void Update(float a_deltaTime) = 0;
	/// <summary>
	/// Draws the component's elements on-screen.
	/// </summary>
	/// <param name="a_pRenderingFramework"> A pointer to the program that renders the object on-screen. </param>
	virtual void Draw(Framework* a_pRenderingFramework) = 0;

	/// <summary>
	/// Returns a pointer to the entity that the component is attached to.
	/// </summary>
	/// <returns> An entity pointer. </returns>
	inline Entity* GetEntity() const;
	/// <summary>
	/// Returns the component's type.
	/// </summary>
	/// <returns> The component type. </returns>
	inline COMPONENT_TYPE GetComponentType() const;

protected:
	Entity* m_pAttachedEntity;
	COMPONENT_TYPE m_componentType;
};

inline Entity* Component::GetEntity() const
{
	return m_pAttachedEntity;
}

inline COMPONENT_TYPE Component::GetComponentType() const
{
	return m_componentType;
}

#endif // !COMPONENT_H.
