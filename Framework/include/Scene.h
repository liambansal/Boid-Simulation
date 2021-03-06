//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/02/2021
//////////////////////////////

#ifndef SCENE_H
#define SCENE_H

// Header includes.
#include <map>

// Forward declarations.
class Camera;
class Entity;
class Shader;

class Scene
{
public:
	Scene();
	~Scene();

	void AddEntity(Entity* a_pNewEntity);
	inline Camera* GetCamera() const;
	inline Entity* GetEntity(unsigned int a_uniqueID);
	// Draws all scene Entity's.
	void Draw(Shader* a_pShader) const;

private:
	Camera* m_pCamera;
	std::map<unsigned int, Entity*> m_sceneEntities;
};

inline Camera* Scene::GetCamera() const
{
	return m_pCamera;
}

inline Entity* Scene::GetEntity(unsigned int a_uniqueID)
{
	return m_sceneEntities.empty() ? nullptr : m_sceneEntities[a_uniqueID];
}

#endif // !SCENE_H.
