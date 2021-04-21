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
typedef std::map<unsigned int, Entity*> EntityMap;

Scene::Scene() : m_uiNumberOfBoids(0),
	m_pCamera(new Camera(glm::vec3(0.0f, 0.0f, 5.0f)))
{}

Scene::~Scene()
{
	for (EntityMap::const_iterator iterator = m_sceneEntities.begin();
		iterator != m_sceneEntities.end();
		++iterator)
	{
		Entity* entity = iterator->second;

		if (entity)
		{
			delete entity;
			entity = nullptr;
		}
	}

	delete m_pCamera;
	m_pCamera = nullptr;
}

// Calls update on all the scene's entities.
void Scene::Update(float a_deltaTime)
{
	EntityMap entities = GetEntityList();

	for (EntityMap::const_iterator iterator = entities.begin(); iterator != entities.end(); ++iterator)
	{
		Entity* entity = iterator->second;

		if (entity)
		{
			entity->Update(a_deltaTime);
		}
	}
}

// Draws all scene's entities.
void Scene::Draw(unsigned int a_screenWidth, unsigned int a_screenHeight, Shader* a_pShader) const
{
	if (m_sceneEntities.empty())
	{
		return;
	}

	// Don't forget to enable shader before setting uniforms.
	a_pShader->use();
	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(GetCamera()->Zoom),
		(float)a_screenWidth / (float)a_screenHeight,
		0.1f,
		100.0f);
	glm::mat4 view = GetCamera()->GetViewMatrix();
	a_pShader->setMat4("projection", projection);
	a_pShader->setMat4("view", view);

	for (auto iterator = m_sceneEntities.cbegin(); iterator != m_sceneEntities.cend(); ++iterator)
	{
		(*iterator).second->Draw(a_pShader);
	}
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
