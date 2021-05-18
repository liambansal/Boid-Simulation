//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Scene.h"
#include "UserInterface.h"

// Forward declarations.
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

	inline int GetBoidCount() const;
	inline unsigned int GetMaximumBoidCount() const;

protected:
	int m_uiBoidCount;
	const unsigned int mc_uiMaximumBoidCount;
	Scene m_scene;

private:
	bool m_bFrameworkInitialised;
	Framework* m_pFramework;
	UserInterface m_userInterface;
};

void Application::SetBoidCount(unsigned int a_boidCount)
{
	m_uiBoidCount = (int)a_boidCount;
}

int Application::GetBoidCount() const
{
	return m_uiBoidCount;
}

unsigned int Application::GetMaximumBoidCount() const
{
	return mc_uiMaximumBoidCount;
}

#endif // !APPLICATION_H.
