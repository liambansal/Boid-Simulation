//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "Component.h"
#include "Entity.h"

Component::Component(Entity* a_pOwner) : m_pAttachedEntity(a_pOwner),
	m_componentType(COMPONENT_TYPE_NONE) {}
