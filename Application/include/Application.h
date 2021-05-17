//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Scene.h"

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

private:
	int m_uiBoidCount;
	const unsigned int mc_uiMaximumBoidCount;
	bool m_bFrameworkInitialised;
	Framework* m_pFramework;
	Scene m_scene;
};

#endif // !APPLICATION_H.
