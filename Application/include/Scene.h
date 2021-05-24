//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef SCENE_H
#define SCENE_H

// Header includes.
#include "Entity.h"
#include <map>
#include "OctTree.h"
#include <string>

// Forward declarations.
class Shader;
class Framework;

class Scene
{
public:
	Scene();
	~Scene();

	// Calls update on all the scene's entities.
	void Update(float a_deltaTime);
	// Draws all scene's entities.
	void Draw(Framework* a_pRenderingFramework) const;
	bool AddEntity(Entity* a_pNewEntity);
	void AddEntities(Entity* a_pNewEntity, unsigned int a_spawnAmount);
	void DestroyEntity(Entity* a_pEntityToDestroy);
	void DestroyEntitiesWithTag(std::string a_entityTag, unsigned int a_destroyAmount);

	// Returns a pointer to an entity using its unique identifier.
	inline Entity* GetEntity(unsigned int a_uniqueID);
	// Returns a reference to a map containing all the scene's entities.
	inline const std::map<unsigned int, Entity*>& GetEntityMap() const;
	// Gets the total number of entities in the scene.
	inline const unsigned int GetEntityCount() const;
	// Gets the number of entities in the scene with a matching tag.
	inline const unsigned int GetEntityCount(std::string a_tag) const;

private:
	unsigned int m_uiEntityCount;
	std::map<unsigned int, Entity*> m_sceneEntities;
	OctTree<Entity, glm::vec4> m_octTree;
};

// Returns a pointer to an entity using its unique identifier.
Entity* Scene::GetEntity(unsigned int a_uniqueID)
{
	return m_sceneEntities.empty() ? nullptr : m_sceneEntities[a_uniqueID];
}

// Returns a reference to a map containing all the scene's entities.
const std::map<unsigned int, Entity*>& Scene::GetEntityMap() const
{
	return m_sceneEntities;
}

// Gets the total number of entities in the scene.
const unsigned int Scene::GetEntityCount() const
{
	return m_uiEntityCount;
}

// Gets the number of entities in the scene with a matching tag.
const unsigned int Scene::GetEntityCount(std::string a_tag) const
{
	// Entities in the scene with a matching tag.
	unsigned int matchingEntities = 0;

	for (std::pair<unsigned int, Entity*> entity : m_sceneEntities)
	{
		if (entity.second->GetTag() == a_tag)
		{
			++matchingEntities;
		}
	}

	return matchingEntities;
}

#endif // !SCENE_H.
