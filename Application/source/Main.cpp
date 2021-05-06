//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

#include "Framework.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "BrainComponent.h"
#include "Entity.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Utilities.h"

int main()
{
	Framework* pFramework = Framework::GetInstance();
	Scene scene;

	if (pFramework)
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
		const unsigned int maximumBoidCount = 60;
		// The absolute value for the maximum spawn distance.
		const int maximumSpawnDistance = 5;

		for (unsigned int i = 0; i < maximumBoidCount; ++i)
		{
			// Create a new boid.
			Entity* pBoid = new Entity();
			// Create transform
			TransformComponent* pTransform = new TransformComponent(pBoid);
			pTransform->SetMatrixRow(MATRIX_ROW::MATRIX_ROW_POSITION_VECTOR,
				glm::vec3(Utilities::RandomRange(-maximumSpawnDistance, maximumSpawnDistance),
				Utilities::RandomRange(-maximumSpawnDistance, maximumSpawnDistance),
				Utilities::RandomRange(-maximumSpawnDistance, maximumSpawnDistance)));
			pBoid->AddComponent(COMPONENT_TYPE_TRANSFORM, static_cast<Component*>(pTransform));
			// create model
			ModelComponent* pModel = new ModelComponent(pBoid);
			pModel->LoadModel("Resources/Models/Low_poly_UFO/Low_poly_UFO.obj");
			pModel->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
			pBoid->AddComponent(COMPONENT_TYPE_MODEL, static_cast<Component*>(pModel));
			// create brain i.e. AI controller
			BrainComponent* pBrain = new BrainComponent(pBoid);
			pBoid->AddComponent(COMPONENT_TYPE_AI, static_cast<Component*>(pBrain));
			scene.AddEntity(pBoid);
		}

		while (isInitialised && glfwWindowShouldClose(pFramework->GetWindow()) == 0)
		{
			glClearColor(0.5f, 0.5f, 0.5f, 0.1f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Update
			pFramework->Update();
			scene.Update(pFramework->GetDeltaTime());

			// Draw
			scene.Draw(pFramework);
			// UI
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			ImGuiIO& io = ImGui::GetIO();
			const ImVec2 windowPosition = ImVec2(1.0f, io.DisplaySize.y * 0.75f);
			ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always);

			// Setup sliders
			if (ImGui::Begin("Slider Menu"))
			{
				io.MouseDrawCursor = true;
				int boidCount = 30;
				ImGui::SliderInt("Boid Count", &boidCount, 0, 60);
			}

			ImGui::End();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(pFramework->GetWindow());
			glfwPollEvents();
		}

		pFramework->Destory();
		delete pFramework;
		pFramework = nullptr;
		return 0;
	}

	return 1;
}