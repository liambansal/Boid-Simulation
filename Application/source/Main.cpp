//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

// Header includes.
#include "Framework.h"
#include "BrainComponent.h"
#include "Entity.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Utilities.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

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
		const int maximumBoidCount = 60;

		for (int i = 0; i < maximumBoidCount; ++i)
		{
			// Create a boid.
			Entity* pBoid = new Entity();
			TransformComponent* pTransform = new TransformComponent(pBoid);
			const int maximumDistance = 5;
			pTransform->SetMatrixRow(MATRIX_ROW::MATRIX_ROW_POSITION_VECTOR,
				glm::vec3(Utilities::RandomRange(-maximumDistance, maximumDistance),
				Utilities::RandomRange(-maximumDistance, maximumDistance),
				Utilities::RandomRange(-maximumDistance, maximumDistance)));
			pBoid->AddComponent(COMPONENT_TYPE_TRANSFORM, static_cast<Component*>(pTransform));
			ModelComponent* pModel = new ModelComponent(pBoid);
			pModel->LoadModel("Resources/Models/Low_poly_UFO/Low_poly_UFO.obj");
			pModel->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
			pBoid->AddComponent(COMPONENT_TYPE_MODEL, static_cast<Component*>(pModel));
			BrainComponent* pBrain = new BrainComponent(pBoid);
			pBoid->AddComponent(COMPONENT_TYPE_AI, static_cast<Component*>(pBrain));
			pFramework->GetScene()->AddEntity(pBoid);
		}

		while (isInitialised && glfwWindowShouldClose(pFramework->GetWindow()) == 0)
		{
			glClearColor(0.5f, 0.5f, 0.5f, 0.1f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			pFramework->Update();
			pFramework->Draw();

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