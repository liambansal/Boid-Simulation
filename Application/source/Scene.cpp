//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "Scene.h"
#include "BrainComponent.h"
#include "Entity.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "LearnOpenGL/camera.h"
#include "LearnOpenGL/shader.h"
#include "ModelComponent.h"
#include "Framework.h"
#include "TransformComponent.h"
#include "Utilities.h"

// Typedefs.
typedef std::pair<unsigned int, Entity*> EntityPair;
typedef std::map<unsigned int, Entity*> EntityMap;

Scene::Scene() : m_uiNumberOfBoids(0)
{}

Scene::~Scene()
{
	for (EntityMap::const_iterator iterator = m_sceneEntities.cbegin();
		iterator != m_sceneEntities.cend();
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

	for (EntityMap::const_iterator iterator = entities.cbegin(); iterator != entities.cend(); ++iterator)
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

	for (std::map<unsigned int, Entity*>::const_iterator iterator = m_sceneEntities.cbegin(); iterator != m_sceneEntities.cend(); ++iterator)
	{
		iterator->second->Draw(a_pRenderingFramework);
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

	if (a_pNewEntity->GetTag() == "Boid")
	{
		++m_uiNumberOfBoids;
	}

	// TODO increase the scenes entity count
}

void Scene::AddEntities(Entity a_EntityToCopy, unsigned int a_spawnAmount)
{
	for (unsigned int i = 0; i < a_spawnAmount; ++i)
	{
		Entity* pNewEntity = new Entity(a_EntityToCopy);
		AddEntity(pNewEntity);
		++m_uiNumberOfBoids;
	}
}

void Scene::DestroyEntity(Entity* a_pEntityToDestroy)
{
	// Check entity exists in the scene.
	if (m_sceneEntities.count(a_pEntityToDestroy->GetID()) > 0)
	{
		m_sceneEntities.erase(a_pEntityToDestroy->GetID());
	}
}

void Scene::DestroyEntitiesWithTag(std::string a_entityTag, unsigned int a_destroyAmount)
{
	// Test whether or not a type of entity exists in the scene.
	bool entityTypeCleared = true;

	for (unsigned int i = 0; i < a_destroyAmount; ++i)
	{
		for (std::map<unsigned int, Entity*>::const_iterator iterator = m_sceneEntities.cbegin();
			iterator != m_sceneEntities.cend();
			++iterator)
		{
			// Assume the entity type doesn't exist until we find one.
			entityTypeCleared = true;

			if (iterator->second->GetTag() == a_entityTag)
			{
				// May be more types of this entity, keep going until desired amount is cleared.
				entityTypeCleared = false;
				m_sceneEntities.erase(iterator);
				--m_uiNumberOfBoids;
				break;
			}
		}

		if (entityTypeCleared)
		{
			return;
		}
	}
}
