//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef SCENE_H
#define SCENE_H

// Header includes.
#include <map>

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

	// Returns a pointer to an entity using its unique identifier.
	inline Entity* GetEntity(unsigned int a_uniqueID);
	// Returns a reference to all entities in the scene.
	inline const std::map<unsigned int, Entity*>& GetEntityList() const;
private:
	unsigned int m_uiNumberOfBoids;
	std::map<unsigned int, Entity*> m_sceneEntities;
};

// Returns a pointer to an entity using its unique identifier.
inline Entity* Scene::GetEntity(unsigned int a_uniqueID)
{
	return m_sceneEntities.empty() ? nullptr : m_sceneEntities[a_uniqueID];
}

// Returns a reference to all entities in the scene.
inline const std::map<unsigned int, Entity*>& Scene::GetEntityList() const
{
	return m_sceneEntities;
}

#endif // !SCENE_H.
