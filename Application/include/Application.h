//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

#ifndef APPLICATION_H
#define APPLICATION_H

// Forward declarations.
class Entity;
class Framework;
class Scene;
class UserInterface;
class TransformComponent;

// Top level class for our boids simulation from which everything else will work from.
class Application
{
public:
	Application();
	~Application();

	void Run();
	void Update();
	void Draw();
	// Creates a new boid entity.
	Entity* CreateBoid();
	// Creates a new obstacle entity.
	Entity* CreateObstacle(TransformComponent* a_pTransform);

	inline void SetBoidCount(unsigned int a_uiBoidCount);

	inline unsigned int GetBoidCount() const;
	inline unsigned int GetMaximumBoidCount() const;

private:
	unsigned int m_uiBoidCount;
	const unsigned int mc_uiMaximumBoidCount;
	float m_fMarkerZOffset;
	const float mc_fMimimumMarkerZOffset;
	const float mc_fMaximumMarkerZOffset;
	bool m_bFrameworkInitialised;
	bool m_bSpawnedObstacle;
	Framework* m_pFramework;
	Scene* m_scene;
	UserInterface* m_userInterface;
	// A 3D cursor existing in world space.
	Entity* m_pWorldCursor;
};

void Application::SetBoidCount(unsigned int a_uiBoidCount)
{
	m_uiBoidCount = a_uiBoidCount;
}

unsigned int Application::GetBoidCount() const
{
	return m_uiBoidCount;
}

unsigned int Application::GetMaximumBoidCount() const
{
	return mc_uiMaximumBoidCount;
}

#endif // !APPLICATION_H.
