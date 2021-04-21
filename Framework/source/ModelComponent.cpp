//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "ModelComponent.h"
#include "Entity.h"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "LearnOpenGL/shader.h"
#include "LearnOpenGL/model.h"
#include "TransformComponent.h"

// Typedefs
typedef Component Parent;

std::map<const char*, Model> ModelComponent::m_loadedModels = std::map<const char*, Model>();

ModelComponent::ModelComponent(Entity* a_owner) : Parent(a_owner),
	m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_pModel(nullptr)
{}

ModelComponent::~ModelComponent()
{}

void ModelComponent::Update(float a_deltaTime)
{}

void ModelComponent::Draw(Shader* a_pShader)
{
	// Get transform component.
	TransformComponent* pTransform = static_cast<TransformComponent*>(m_pAttachedEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (!m_pModel || !a_pShader || !pTransform)
	{
		// Early out if any pointers are null.
		return;
	}

	glm::mat4 modelMatrix = pTransform->GetMatrix();
	// It's a bit too big for our scene, so scale it down.
	modelMatrix = glm::scale(modelMatrix, m_scale);
	a_pShader->setMat4("model", modelMatrix);
	// Render the loaded model.
	m_pModel->Draw(*a_pShader);
}

void ModelComponent::LoadModel(const char* a_pFilepath)
{
	// Check if model has been loaded from file already.
	if (m_loadedModels.count(a_pFilepath))
	{
		// Find the already existing model.
		m_pModel = &m_loadedModels.at(a_pFilepath);
	}
	else
	{
		// Load an instance of a new model.
		m_pModel = new Model(a_pFilepath);
		m_loadedModels.insert(std::pair<const char*, Model>(a_pFilepath, *m_pModel));
	}
}


void ModelComponent::SetScale(glm::vec3 a_scale)
{
	m_scale = a_scale;
}