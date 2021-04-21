//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef MODEL_COMPONENT_H
#define MODEL_COMPONENT_H

// Header includes.
#include "Component.h"
#include "glm/glm.hpp"
#include <map>

// Forward declarations.
class Entity;
class Model;

class ModelComponent : public Component
{
public:
	ModelComponent(Entity* a_owner);
	~ModelComponent();

	virtual void Update(float a_deltaTime);
	virtual void Draw(Shader* a_pShader);
	void LoadModel(const char* a_pFilepath);

	void SetScale(glm::vec3 a_scale);

private:
	glm::vec3 m_scale;
	Model* m_pModel;
	static std::map<const char*, Model> m_loadedModels;
};

#endif // !MODEL_COMPONENT_H.
