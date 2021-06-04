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
class Framework;
class Model;

// Loads and displays a 3D model using Assimp.
class ModelComponent : public Component
{
public:
	ModelComponent(Entity* a_owner);
	ModelComponent(Entity* a_owner,
		ModelComponent& a_rModelToCopy);
	~ModelComponent()
	{}

	virtual void Update(float a_fDeltaTime)
	{}
	virtual void Draw(Framework* a_pRenderingFramework);
	void LoadModel(const char* a_pFilepath);

	void SetScale(glm::vec3 a_scale);

	inline Model* GetModel();

private:
	glm::mat4 m_scaleMatrix;
	Model* m_pModel;
	static std::map<const char*, Model*> ms_loadedModels;
};

Model* ModelComponent::GetModel()
{
	return m_pModel;
}

#endif // !MODEL_COMPONENT_H.
