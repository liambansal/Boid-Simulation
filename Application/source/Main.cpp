//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

#include "Framework.h"

int main()
{
	Framework* pFramework = Framework::GetInstance();

	if (pFramework != nullptr)
	{
		const int width = 1080;
		const int height = 720;
		const char* name = "Hello Library";
		bool isInitialised = pFramework->Initialize(name,
			width,
			height,
			"Resources/Shaders/model_loading.vs",
			"Resources/Shaders/model_loading.fs",
			"Resources/Models/Nanosuit/nanosuit.obj");

		if (isInitialised)
		{
			pFramework->Update();
			pFramework->Destory();
		}

		delete pFramework;
		pFramework = nullptr;
		return 0;
	}

	return 1;
}