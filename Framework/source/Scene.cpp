//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "Scene.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "LearnOpenGL/camera.h"
#include "LearnOpenGL/shader.h"

// Typedefs.
typedef std::pair<unsigned int, Entity*> EntityPair;

Scene::Scene() : m_pCamera(new Camera(glm::vec3(0.0f, 0.0f, 0.3f)))
{}

Scene::~Scene()
{
	delete m_pCamera;
	m_pCamera = nullptr;
}

void Scene::AddEntity(Entity* a_pNewEntity)
{
	if (m_sceneEntities.count(a_pNewEntity->GetID()) > 0)
	{
		// Entity is already in the scene, return.
		return;
	}
	
	m_sceneEntities.insert(EntityPair(a_pNewEntity->GetID(), a_pNewEntity));
}

// Draws all scene Entity's.
void Scene::Draw(Shader* a_pShader) const
{
	if (m_sceneEntities.empty())
	{
		return;
	}

	for (auto iterator = m_sceneEntities.cbegin(); iterator != m_sceneEntities.cend(); ++iterator)
	{
		(*iterator).second->Draw(a_pShader);
	}
}