//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef SCENE_H
#define SCENE_H

// Header includes.
#include "Entity.h"
#include "glm/glm.hpp"
#include <map>
#include "OctTree.h"
#include <string>

// Forward declarations.
class Shader;
class Framework;

/// <summary>
/// Acts as a virtual environment for creating and managing entities.
/// </summary>
class Scene
{
public:
	Scene();
	~Scene();

	/// <summary>
	/// Updates all of the scene's entities.
	/// Always call once per frame.
	/// </summary>
	/// <param name="a_deltaTime"> The amount of time (in seconds) that has passed since the last frame and current frame. </param>
	void Update(float a_deltaTime);
	/// <summary>
	/// Draws all of the scene's elements on-screen.
	/// </summary>
	/// <param name="a_pRenderingFramework"> The program that handles drawing the scene's elements. </param>
	void Draw(Framework* a_pRenderingFramework) const;
	/// <summary>
	/// Registers a single entity with the scene.
	/// </summary>
	/// <param name="a_pNewEntity"> The entity instance to associate with the scene. </param>
	/// <returns> True if the entity was successfully added to the scene. </returns>
	bool AddEntity(Entity* a_pNewEntity);
	void AddEntities(Entity* a_pNewEntity, unsigned int a_spawnAmount);
	void DestroyEntity(Entity* a_pEntityToDestroy);
	/// <summary>
	/// Destroys a number of entities within the scene that have a specific tag.
	/// </summary>
	/// <param name="a_entityTag"> Entities with this tag will be destroyed. </param>
	/// <param name="a_destroyAmount"> The number of entities to destroy. </param>
	void DestroyEntitiesWithTag(std::string a_entityTag, unsigned int a_destroyAmount);

	/// <summary>
	/// Finds an entity by searching for its ID.
	/// </summary>
	/// <param name="a_uniqueID"> The ID that belongs to the entity to search for. </param>
	/// <returns> A pointer to the entity that was searched for, if it exists. </returns>
	inline Entity* GetEntity(unsigned int a_uniqueID);
	/// <summary>
	/// Returns a reference to a collection containing all of the scene's entities.
	/// </summary>
	/// <returns> A collection containing a pointer to each entity within the scene. </returns>
	inline const std::map<unsigned int, Entity*>& GetAllEntities() const;
	/// <summary>
	/// Returns the total number of entities in the scene.
	/// </summary>
	/// <returns> The number of entities within the scene. </returns>
	inline const unsigned int GetEntityCount() const;
	/// <summary>
	/// Returns the number of entities in the scene that have a specific tag.
	/// </summary>
	/// <param name="a_tag"> The function only counts entities with this tag. </param>
	/// <returns> The number of matching entities within the scene. </returns>
	inline const unsigned int GetEntityCount(std::string a_tag) const;
	inline const OctTree<Entity, glm::vec3>& GetOctTree() const;

private:
	/// <summary>
	/// The number of entities that exist within the scene.
	/// </summary>
	unsigned int m_uiEntityCount;
	/// <summary>
	/// A collection of all the scene's entities.
	/// </summary>
	std::map<unsigned int, Entity*> m_sceneEntities;
	/// <summary>
	/// The oct-tree helps the scene to track the positions of its entities and update them
	/// (regarding collisions, queries, etc.) in a performant manner.
	/// </summary>
	OctTree<Entity, glm::vec3> m_octTree;
};

Entity* Scene::GetEntity(unsigned int a_uniqueID)
{
	return m_sceneEntities.empty() ? nullptr : m_sceneEntities[a_uniqueID];
}

const std::map<unsigned int, Entity*>& Scene::GetAllEntities() const
{
	return m_sceneEntities;
}

const unsigned int Scene::GetEntityCount() const
{
	return m_uiEntityCount;
}

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

const OctTree<Entity, glm::vec3>& Scene::GetOctTree() const
{
	return m_octTree;
}

#endif // !SCENE_H.
