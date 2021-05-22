//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Scene.h"
#include "UserInterface.h"

// Forward declarations.
class Entity;
class Framework;

class Application
{
public:
	Application();
	~Application();

	void Run();
	void Update();
	void Draw();
	Entity* CreateBoid();

	inline void SetBoidCount(unsigned int a_boidCount);

	inline unsigned int GetBoidCount() const;
	inline unsigned int GetMaximumBoidCount() const;

private:
	unsigned int m_uiBoidCount;
	const unsigned int mc_uiMaximumBoidCount;
	float m_fMarkerZOffset;
	const float mc_fMimimumMarkerZOffset;
	const float mc_fMaximumMarkerZOffset;
	bool m_bFrameworkInitialised;
	Framework* m_pFramework;
	Scene m_scene;
	UserInterface m_userInterface;
	// A 3D cursor existing in world space.
	Entity* m_pWorldCursor;
};

void Application::SetBoidCount(unsigned int a_boidCount)
{
	m_uiBoidCount = a_boidCount;
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
