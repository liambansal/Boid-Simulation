//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef COMPONENT_H
#define COMPONENT_H

// Forwards declarations.
class Entity;
class Shader;

enum COMPONENT_TYPE
{
	COMPONENT_TYPE_NONE,
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_MODEL,
	COMPONENT_TYPE_AI,
	COMPONENT_TYPE_COUNT
};

class Component
{
public:
	Component(Entity* a_pOwner);

	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw(Shader* a_pShader) = 0;

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
