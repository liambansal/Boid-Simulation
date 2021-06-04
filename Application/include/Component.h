//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef COMPONENT_H
#define COMPONENT_H

// Forwards declarations.
class Entity;
class Framework;

enum COMPONENT_TYPE
{
	COMPONENT_TYPE_NONE,
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_MODEL,
	COMPONENT_TYPE_COLLIDER,
	COMPONENT_TYPE_BRAIN,
	COMPONENT_TYPE_COUNT
};

// Generic class for inheriting only.
// Provides a base default functionality for all components.
class Component
{
public:
	Component(Entity* a_pOwner);

	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw(Framework* a_pRenderingFramework) = 0;

	// Returns the entity this component is attached to.
	inline Entity* GetEntity() const;
	inline COMPONENT_TYPE GetComponentType() const;

protected:
	Entity* m_pAttachedEntity;
	COMPONENT_TYPE m_componentType;
};

// Returns the entity this component is attached to.
inline Entity* Component::GetEntity() const
{
	return m_pAttachedEntity;
}

inline COMPONENT_TYPE Component::GetComponentType() const
{
	return m_componentType;
}

#endif // !COMPONENT_H.
