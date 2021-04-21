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
#include "Utilities.h"

int main()
{
	Framework* pFramework = Framework::GetInstance();

	if (pFramework != nullptr)
	{
		// Seed rand number generator.
		srand(time(nullptr));

		const int width = 1080;
		const int height = 720;
		const char* name = "Hello Library";
		bool isInitialised = pFramework->Initialize(name,
			width,
			height,
			"Resources/Shaders/model_loading.vs",
			"Resources/Shaders/model_loading.fs");
		const int maximumBoidCount = 5;

		for (int i = 0; i < maximumBoidCount; ++i)
		{
			// Create a boid.
			Entity* pBoid = new Entity();
			TransformComponent* pTransform = new TransformComponent(pBoid);
			const int maximumDistance = 10;
			pTransform->SetPosition(MATRIX_ROW::MATRIX_ROW_POSITION_VECTOR, glm::vec3(Utilities::RandomRange(0, maximumDistance),
				Utilities::RandomRange(0, maximumDistance),
				Utilities::RandomRange(0, maximumDistance)));
			ModelComponent* pModel = new ModelComponent(pBoid);
			pModel->LoadModel("Resources/Models/Nanosuit/nanosuit.obj");
			pModel->SetScale(glm::vec3(0.04f, 0.04f, 0.04f));
			pBoid->AddComponent(static_cast<Component*>(pTransform));
			pBoid->AddComponent(static_cast<Component*>(pModel));
			pFramework->GetScene()->AddEntity(pBoid);
		}

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