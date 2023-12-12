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

/// <summary>
/// Renders a 3D model on-screen for an entity.
/// </summary>
class ModelComponent : public Component {
public:
	ModelComponent(Entity* a_owner);
	ModelComponent(Entity* a_owner,
		ModelComponent& a_rModelToCopy);
	~ModelComponent() {}

	virtual void Update(float a_fDeltaTime);
	virtual void Draw(Framework* a_pRenderingFramework);
	/// <summary>
	/// Loads a model to represent the entity.
	/// </summary>
	/// <param name="a_pFilepath"></param>
	void LoadModel(const char* a_pFilepath);
	/// <summary>
	/// Sets the width, height, and depth of the entity's model.
	/// </summary>
	/// <param name="a_scale"> The model's new scale. </param>
	void SetScale(glm::vec3 a_scale);
	inline void SetOffset(glm::vec3 a_offset);

	inline glm::vec3 GetOffset() const { return m_positionOffset; }
	inline Model* GetModel();

private:
	/// <summary>
	/// A matrix that represents the models dimensions.
	/// </summary>
	glm::mat4 m_scaleMatrix;
	/// <summary>
	/// The difference between the entity's position and where the model is rendered.
	/// </summary>
	glm::vec3 m_positionOffset;
	/// <summary>
	/// The model that represents the entity.
	/// </summary>
	Model* m_pModel;
	/// <summary>
	/// A collection of all the models that have been loaded by an entity.
	/// </summary>
	static std::map<const char*, Model*> ms_loadedModels;
};

void ModelComponent::SetOffset(glm::vec3 a_offset) {
	m_positionOffset = a_offset;
}

Model* ModelComponent::GetModel() {
	return m_pModel;
}

#endif // !MODEL_COMPONENT_H.
