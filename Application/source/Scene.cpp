//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

// File's header.
#include "Scene.h"
#include "BrainComponent.h"
#include "Boundary.h"
#include "glm/ext.hpp"
#include "LearnOpenGL/camera.h"
#include "LearnOpenGL/shader.h"
#include "ModelComponent.h"
#include "Framework.h"
#include "ColliderComponent.h"
#include "Utilities.h"

// Typedefs.
typedef std::map<unsigned int, Entity*> EntityMap;
typedef std::pair<unsigned int, Entity*> EntityPair;

Scene::Scene() : m_uiEntityCount(0),
	m_sceneEntities(),
	m_octTree(4,
		Boundary<glm::vec3>(new glm::vec3(0.0f),
		glm::vec3(10.0f))) {}

Scene::~Scene() {
	for (EntityMap::const_iterator iterator = m_sceneEntities.cbegin();
		iterator != m_sceneEntities.cend();
		++iterator) {
		Entity* entity = iterator->second;

		if (entity) {
			delete entity;
			entity = nullptr;
		}
	}
}

void Scene::Update(float a_fDeltaTime) {
	for (EntityPair entity : GetAllEntities()) {
		entity.second->Update(a_fDeltaTime);
	}
}

void Scene::Draw(Framework* a_pRenderingFramework) {
	m_octTree.Draw();

	if (!m_sceneEntities.empty()) {
		for (EntityPair entity : m_sceneEntities) {
			entity.second->Draw(a_pRenderingFramework);
		}
	}
}

bool Scene::AddEntity(Entity* a_pNewEntity) {
	// Checks if the entity already exists within the scene.
	if (m_sceneEntities.find(a_pNewEntity->GetID()) != m_sceneEntities.cend()) {
		return false;
	}

	m_sceneEntities.insert(EntityPair(a_pNewEntity->GetID(), a_pNewEntity));
	++m_uiEntityCount;
	ColliderComponent* pCollider = static_cast<ColliderComponent*>(a_pNewEntity->GetComponentOfType(COMPONENT_TYPE_COLLIDER));

	if (pCollider) {
		m_octTree.InsertObject(a_pNewEntity, *pCollider->GetBoundary());
	}

	return true;
}

void Scene::AddEntities(Entity* a_pEntityToCopy, unsigned int a_spawnAmount) {
	// Try adding entity in case it isn't already in the scene.
	if (AddEntity(a_pEntityToCopy)) {
		--a_spawnAmount;
	}

	for (unsigned int i = 0; i < a_spawnAmount; ++i) {
		AddEntity(new Entity(*a_pEntityToCopy, this));
	}
}

void Scene::DestroyEntity(Entity* a_pEntityToDestroy) {
	if (m_sceneEntities.find(a_pEntityToDestroy->GetID()) != m_sceneEntities.cend()) {
		m_sceneEntities.erase(a_pEntityToDestroy->GetID());
		--m_uiEntityCount;
	}
}

void Scene::DestroyEntitiesWithTag(std::string a_entityTag, unsigned int a_uiDestroyAmount) {
	if (a_entityTag.empty() || a_uiDestroyAmount == 0) {
		return;
	}

	for (EntityMap::const_iterator iterator = m_sceneEntities.cbegin();
		iterator != m_sceneEntities.cend();) {
		if (iterator->second->GetTag() != a_entityTag) {
			++iterator;
			continue;
		}

		EntityMap::const_iterator entityToDestroy = iterator++;
		m_sceneEntities.erase(entityToDestroy);
		--m_uiEntityCount;
		--a_uiDestroyAmount;

		if (a_uiDestroyAmount == 0) {
			return;
		}
	}
}
