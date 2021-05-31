//////////////////////////////
// Author: Liam Bansal
// Date Created: 30/05/2021
//////////////////////////////

#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

// Header includes.
#include "Component.h"
#include "Boundary.h"

// Forward declarations.
class Entity;

class ColliderComponent : public Component
{
public:
	ColliderComponent(Entity* a_pOwner);
	ColliderComponent(Entity* a_pOwner,
		ColliderComponent& a_rColliderToCopy);
	~ColliderComponent()
	{}

	virtual void Update(float a_fDeltaTime)
	{}
	virtual void Draw(Framework* a_pRenderingFramework)
	{}

private:
	Boundary m_collider;
};

#endif // !COLLIDER_COMPONENT_H
