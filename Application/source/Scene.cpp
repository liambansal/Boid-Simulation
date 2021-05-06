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
#include "ModelComponent.h"
#include "Framework.h"

// Typedefs.
typedef std::pair<unsigned int, Entity*> EntityPair;
typedef std::map<unsigned int, Entity*> EntityMap;

Scene::Scene() : m_uiNumberOfBoids(0)
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
void Scene::Draw(Framework* a_pRenderingFramework) const
{
	if (m_sceneEntities.empty())
	{
		return;
	}

	for (auto iterator = m_sceneEntities.cbegin(); iterator != m_sceneEntities.cend(); ++iterator)
	{
		if (iterator->second->GetComponentOfType(COMPONENT_TYPE_MODEL)->GetComponentType() == COMPONENT_TYPE_MODEL)
		{
			ModelComponent* modelComponent = static_cast<ModelComponent*>(iterator->second->GetComponentOfType(COMPONENT_TYPE_MODEL));
			a_pRenderingFramework->Draw(modelComponent->GetModel());
		}
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
