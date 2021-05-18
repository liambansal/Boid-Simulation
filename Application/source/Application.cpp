//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

// File's header.
#include "Application.h"
#include "Framework.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "BrainComponent.h"
#include "Entity.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "Utilities.h"

Application::Application() : m_uiBoidCount(60),
	mc_uiMaximumBoidCount(60),
	m_bFrameworkInitialised(false),
	m_pFramework(Framework::GetInstance()),
	m_scene()
{
	if (m_pFramework)
	{
		// Seed rand number generator.
		srand(time(nullptr));
		const int width = 1080;
		const int height = 720;
		const char* name = "Boids Simulation";
		m_bFrameworkInitialised = m_pFramework->Initialize(name,
			width,
			height,
			"Resources/Shaders/model_loading.vs",
			"Resources/Shaders/model_loading.fs");
		m_scene.AddEntities(CreateBoid(), mc_uiMaximumBoidCount);
	}
}

Application::~Application()
{
	m_pFramework->Destory();
	delete m_pFramework;
	m_pFramework = nullptr;
}

void Application::Run()
{
	while (m_bFrameworkInitialised && glfwWindowShouldClose(m_pFramework->GetWindow()) == 0)
	{
		Update();
		Draw();
	}
}

void Application::Update()
{
	m_pFramework->Update();
	m_scene.Update(m_pFramework->GetDeltaTime());
}

void Application::Draw()
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw scene entities.
	m_scene.Draw(m_pFramework);

#pragma region UI Setup
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
		ImGui::SliderInt("Boid Count", &m_uiBoidCount, 0, 60);

		if (m_uiBoidCount > m_scene.GetBoidCount())
		{
			m_scene.AddEntities(CreateBoid(), m_uiBoidCount - m_scene.GetBoidCount());
		}
		else if (m_uiBoidCount < m_scene.GetBoidCount())
		{
			m_scene.DestroyEntitiesWithTag("Boid", m_scene.GetBoidCount() - m_uiBoidCount);
		}
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

	glfwSwapBuffers(m_pFramework->GetWindow());
	glfwPollEvents();
}

// Creates a new boid entity.
Entity* Application::CreateBoid()
{
	Entity* pBoid = new Entity();
	// Create transform
	TransformComponent* pTransform = new TransformComponent(pBoid);
	// The absolute value for the maximum spawn distance.
	const int maximumSpawnDistance = 5;
	pTransform->SetMatrixRow(MATRIX_ROW::MATRIX_ROW_POSITION_VECTOR,
		glm::vec3(Utilities::RandomRange(-maximumSpawnDistance, maximumSpawnDistance),
			Utilities::RandomRange(-maximumSpawnDistance, maximumSpawnDistance),
			Utilities::RandomRange(-maximumSpawnDistance, maximumSpawnDistance)));
	pBoid->AddComponent(COMPONENT_TYPE_TRANSFORM, static_cast<Component*>(pTransform));
	// create model
	ModelComponent* pModel = new ModelComponent(pBoid);
	pModel->LoadModel("Resources/Models/Low_poly_UFO/Low_poly_UFO.obj");
	const float scaleScalar = 0.01f;
	pModel->SetScale(glm::vec3(scaleScalar));
	pBoid->AddComponent(COMPONENT_TYPE_MODEL, static_cast<Component*>(pModel));
	// create brain i.e. AI controller
	BrainComponent* pBrain = new BrainComponent(pBoid, &m_scene);
	pBoid->AddComponent(COMPONENT_TYPE_BRAIN, static_cast<Component*>(pBrain));
	pBoid->SetTag("Boid");
	return pBoid;
}