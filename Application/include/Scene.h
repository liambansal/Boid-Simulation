//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef SCENE_H
#define SCENE_H

// Header includes.
#include <map>
#include <string>

// Forward declarations.
class Entity;
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
	void AddEntity(Entity* a_pNewEntity);
	void AddEntities(Entity a_newEntity, unsigned int a_spawnAmount);
	void DestroyEntity(Entity* a_pEntityToDestroy);
	void DestroyEntitiesWithTag(std::string a_entityTag, unsigned int a_destroyAmount);

	// Returns a pointer to an entity using its unique identifier.
	inline Entity* GetEntity(unsigned int a_uniqueID);
	// Returns a reference to all entities in the scene.
	inline const std::map<unsigned int, Entity*>& GetEntityList() const;
	inline const unsigned int GetBoidCount() const;

private:
	unsigned int m_uiEntityCount;
	std::map<unsigned int, Entity*> m_sceneEntities;
};

// Returns a pointer to an entity using its unique identifier.
Entity* Scene::GetEntity(unsigned int a_uniqueID)
{
	return m_sceneEntities.empty() ? nullptr : m_sceneEntities[a_uniqueID];
}

// Returns a reference to all entities in the scene.
const std::map<unsigned int, Entity*>& Scene::GetEntityList() const
{
	return m_sceneEntities;
}

const unsigned int Scene::GetBoidCount() const
{
	return m_uiEntityCount;
}

#endif // !SCENE_H.
