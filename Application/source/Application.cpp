//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

// File's header.
#include "Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "BrainComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"
#include "LearnOpenGL/camera.h"
#include "Framework.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "UserInterface.h"
#include "Utilities.h"

Application::Application() : m_uiBoidCount(50),
	mc_uiMaximumBoidCount(5000),
	m_fMarkerZOffset(10.0f),
	mc_fMimimumMarkerZOffset(0.5f),
	mc_fMaximumMarkerZOffset(50.0f),
	m_bFrameworkInitialised(false),
	m_bSpawnedObstacle(false),
	m_pFramework(Framework::GetInstance()),
	m_scene(),
	m_userInterface(this),
	m_pWorldCursor(new Entity())
{
	if (m_pFramework)
	{
		// Seed rand number generator.
		srand(time(nullptr));
		const char* name = "Boids Simulation";
		m_bFrameworkInitialised = m_pFramework->Initialize(name,
			m_pFramework->GetScreenWidth(),
			m_pFramework->GetScreenHeight(),
			"Resources/Shaders/model_loading.vs",
			"Resources/Shaders/model_loading.fs");
		TransformComponent* pTransform = new TransformComponent(m_pWorldCursor);
		pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR,
			m_pFramework->GetCamera()->Position + m_pFramework->GetCamera()->Front * m_fMarkerZOffset);
		m_pWorldCursor->AddComponent(COMPONENT_TYPE_TRANSFORM, static_cast<Component*>(pTransform));
		ModelComponent* pModel = new ModelComponent(m_pWorldCursor);
		pModel->LoadModel("Resources/Models/Low_poly_UFO/Low_poly_UFO.obj");
		const float scaleScalar = 0.01f;
		pModel->SetScale(glm::vec3(scaleScalar));
		m_pWorldCursor->AddComponent(COMPONENT_TYPE_MODEL, static_cast<Component*>(pModel));
		m_pWorldCursor->SetTag("Marker");
		m_scene.AddEntity(m_pWorldCursor);
		m_scene.AddEntities(CreateBoid(), m_uiBoidCount);
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
	std::string boidTag = "Boid";

	if (m_uiBoidCount > m_scene.GetEntityCount(boidTag))
	{
		m_scene.AddEntities(CreateBoid(), m_uiBoidCount - m_scene.GetEntityCount(boidTag));
	}
	else if (m_uiBoidCount < m_scene.GetEntityCount(boidTag))
	{
		m_scene.DestroyEntitiesWithTag(boidTag, m_scene.GetEntityCount(boidTag) - m_uiBoidCount);
	}

	TransformComponent* markerTransform = static_cast<TransformComponent*>(m_pWorldCursor->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (markerTransform)
	{
		// Update the marker's position each frame.
		markerTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR,
			m_pFramework->GetCamera()->Position + m_pFramework->GetCamera()->Front * m_fMarkerZOffset);

		if (!m_bSpawnedObstacle && glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_3) == GLFW_PRESS)
		{
			Entity* pObstacle = new Entity();
			// add a new transform component.
			pObstacle->AddComponent(COMPONENT_TYPE_TRANSFORM,
				static_cast<Component*>(new TransformComponent(pObstacle, *markerTransform)));
			// create a model to visualise the obstacle.
			ModelComponent* pModel = new ModelComponent(pObstacle);
			pModel->LoadModel("Resources/Models/Low_poly_UFO/Low_poly_UFO.obj");
			const float scaleScalar = 0.01f;
			pModel->SetScale(glm::vec3(scaleScalar));
			pObstacle->AddComponent(COMPONENT_TYPE_MODEL, static_cast<Component*>(pModel));
			ColliderComponent* pCollider = new ColliderComponent(pObstacle,
				&m_scene.GetOctTree());
			const float dimensionsScale = 2.0f;
			pCollider->SetDimensions(dimensionsScale);
			pObstacle->AddComponent(COMPONENT_TYPE_COLLIDER, pCollider);
			pObstacle->SetTag("Obstacle");
			m_scene.AddEntity(pObstacle);
			m_bSpawnedObstacle = true;
		}
		else if (glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_3) == GLFW_RELEASE)
		{
			m_bSpawnedObstacle = false;
		}

		if (glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_1) == GLFW_PRESS)
		{
			--m_fMarkerZOffset;
		}
		else if (glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_2) == GLFW_PRESS)
		{
			++m_fMarkerZOffset;
		}

		if (m_fMarkerZOffset > mc_fMaximumMarkerZOffset)
		{
			m_fMarkerZOffset = mc_fMaximumMarkerZOffset;
		}
		else if (m_fMarkerZOffset < mc_fMimimumMarkerZOffset)
		{
			m_fMarkerZOffset = mc_fMimimumMarkerZOffset;
		}
	}

	m_scene.Update(m_pFramework->GetDeltaTime());
}

void Application::Draw()
{
	const GLfloat rgbColourValues = 0.5f;
	const GLfloat alphaColourValue = 0.1f;
	glClearColor(rgbColourValues,
		rgbColourValues,
		rgbColourValues,
		alphaColourValue);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw scene entities.
	m_scene.Draw(m_pFramework);
	// Draw UI overlay.
	m_userInterface.Draw();
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
	const int absoluteXDistance = m_scene.GetOctTree().GetBoundary().GetDimensions().x;
	const int absoluteYDistance = m_scene.GetOctTree().GetBoundary().GetDimensions().y;
	const int absoluteZDistance = m_scene.GetOctTree().GetBoundary().GetDimensions().z;
	pTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR,
		glm::vec3(Utilities::RandomRange(-absoluteXDistance, absoluteXDistance),
			Utilities::RandomRange(-absoluteYDistance, absoluteYDistance),
			Utilities::RandomRange(-absoluteZDistance, absoluteZDistance)));
	pBoid->AddComponent(COMPONENT_TYPE_TRANSFORM, static_cast<Component*>(pTransform));
	// create model
	ModelComponent* pModel = new ModelComponent(pBoid);
	pModel->LoadModel("Resources/Models/Low_poly_UFO/Low_poly_UFO.obj");
	const float scaleScalar = 0.01f;
	pModel->SetScale(glm::vec3(scaleScalar));
	pBoid->AddComponent(COMPONENT_TYPE_MODEL, static_cast<Component*>(pModel));
	// Create a collider for collisions.
	ColliderComponent* pCollider = new ColliderComponent(pBoid,
		&m_scene.GetOctTree());
	pBoid->AddComponent(COMPONENT_TYPE_COLLIDER, pCollider);
	// create brain i.e. AI controller
	BrainComponent* pBrain = new BrainComponent(pBoid, &m_scene);
	pBoid->AddComponent(COMPONENT_TYPE_BRAIN, static_cast<Component*>(pBrain));
	pBoid->SetTag("Boid");
	return pBoid;
}