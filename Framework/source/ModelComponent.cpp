//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "ModelComponent.h"
#include "Entity.h"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "LearnOpenGL/model.h"
#include "TransformComponent.h"

// Typedefs
typedef Component Parent;

ModelComponent::ModelComponent(Entity* a_owner) : Parent(a_owner),
	m_scale(glm::vec3(1.0f, 1.0f, 1.0f)),
	m_pModel(nullptr)
{}

ModelComponent::~ModelComponent()
{
	delete m_pModel;
	m_pModel = nullptr;
}

void ModelComponent::Update(float a_deltaTime)
{}

void ModelComponent::Draw(Shader* a_pShader)
{
	// Get transform component.
	TransformComponent* pTransform = static_cast<TransformComponent*>(m_pAttachedEntity->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (m_pModel == nullptr || a_pShader == nullptr || pTransform == nullptr)
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

void ModelComponent::LoadModel(const char* a_pModelFilepath)
{
	m_pModel = new Model(a_pModelFilepath);
}


void ModelComponent::SetScale(glm::vec3 a_scale)
{
	m_scale = a_scale;
}