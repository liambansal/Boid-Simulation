//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

// Header includes.
#include "Framework.h"
#include "Entity.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "TransformComponent.h"

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
			"Resources/Shaders/model_loading.fs");

		// Create a 3D model entity.
		Entity entity;
		TransformComponent transform(&entity);
		entity.AddComponent(static_cast<Component*>(&transform));
		ModelComponent model(&entity);
		model.LoadModel("Resources/Models/Nanosuit/nanosuit.obj");
		entity.AddComponent(static_cast<Component*>(&model));
		pFramework->GetScene()->AddEntity(&entity);

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