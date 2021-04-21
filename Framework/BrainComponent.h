//////////////////////////////
// Author: Liam Bansal
// Date Created: 21/04/2021
//////////////////////////////

#ifndef BRAIN_COMPONENT_H
#define BRAIN_COMPONENT_H

#include "Component.h"
#include "glm/glm.hpp"

class Entity;

class BrainComponent : public Component
{
public:
	BrainComponent(Entity* a_pOwner);

	virtual void Update(float a_deltaTime);
	virtual void Draw(Shader a_pShader);

private:
	glm::vec3 CalculateSeekForce(const glm::vec3& a_v3Target,
		const glm::vec3& a_v3CurrentPosition) const;
	glm::vec3 CalculateFleeForce(const glm::vec3& a_v3Target,
		const glm::vec3& a_v3CurrentPosition) const;
	glm::vec3 CalculateWanderForce(const glm::vec3& a_v3Forward,
		const glm::vec3& a_v3CurrentPosition) const;

	glm::vec3 m_velocity;
};

#endif // !BRAIN_COMPONENT_H
