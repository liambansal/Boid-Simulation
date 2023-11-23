//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

// File's header.
#include "Application.h"
#include "BrainComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"
#include "LearnOpenGL/camera.h"
#include "Framework.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "ModelComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "UserInterface.h"
#include "Utilities.h"

Application::Application() : m_uiBoidCount(50),
	mc_uiMaximumBoidCount(500),
	m_fMarkerZOffset(10.0f),
	mc_fMimimumMarkerZOffset(0.5f),
	mc_fMaximumMarkerZOffset(50.0f),
	m_bFrameworkInitialised(false),
	m_bPaused(false),
	m_bSpawnedObstacle(false),
	m_pFramework(Framework::GetInstance()),
	m_pScene(new Scene()),
	m_pUserInterface(new UserInterface(this)),
	m_pWorldCursor(new Entity()) {
	if (!m_pFramework) {
		return;
	}

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
	m_pScene->AddEntity(m_pWorldCursor);
	m_pScene->AddEntities(CreateBoid(), m_uiBoidCount);
}

Application::~Application() {
	m_pFramework->Destory();
	delete m_pFramework;
	m_pFramework = nullptr;
}

void Application::Run() {
	while (!CloseApplication()) {
		Update();
		Draw();
	}
}

bool Application::CloseApplication() const {
	if (!m_bFrameworkInitialised || glfwWindowShouldClose(m_pFramework->GetWindow()) != 0) {
		return true;
	}

	return false;
}

void Application::Update() {
	if (!m_pScene) {
		return;
	}

	m_pFramework->Update();
	ProcessInput();

	if (!m_bPaused || m_bUpdateOnce) {
		m_pScene->Update(m_pFramework->GetDeltaTime());
		// Reset after updating the application once.
		m_bUpdateOnce = false;
	}
}

void Application::ProcessInput() {
	TransformComponent* pMarkerTransform = static_cast<TransformComponent*>(m_pWorldCursor->GetComponentOfType(COMPONENT_TYPE_TRANSFORM));

	if (pMarkerTransform) {
		// Update the marker's position each frame.
		pMarkerTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR,
			m_pFramework->GetCamera()->Position + m_pFramework->GetCamera()->Front * m_fMarkerZOffset);

		if (!m_bSpawnedObstacle && glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_3) == GLFW_PRESS) {
			CreateObstacle(*pMarkerTransform->GetPosition());
		} else if (glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_3) == GLFW_RELEASE) {
			m_bSpawnedObstacle = false;
		}
	}

	if (glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_1) == GLFW_PRESS) {
		--m_fMarkerZOffset;
	} else if (glfwGetKey(m_pFramework->GetWindow(), GLFW_KEY_2) == GLFW_PRESS) {
		++m_fMarkerZOffset;
	}

	if (m_fMarkerZOffset > mc_fMaximumMarkerZOffset) {
		m_fMarkerZOffset = mc_fMaximumMarkerZOffset;
	} else if (m_fMarkerZOffset < mc_fMimimumMarkerZOffset) {
		m_fMarkerZOffset = mc_fMimimumMarkerZOffset;
	}
}

void Application::Draw() {
	if (!m_pScene || !m_pUserInterface) {
		return;
	}

	const GLfloat rgbColourValues = 0.5f;
	const GLfloat alphaColourValue = 0.1f;
	glClearColor(rgbColourValues,
		rgbColourValues,
		rgbColourValues,
		alphaColourValue);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw scene entities.
	m_pScene->Draw(m_pFramework);
	// Draw UI overlay.
	m_pUserInterface->Draw();
	glfwSwapBuffers(m_pFramework->GetWindow());
	glfwPollEvents();
}

Entity* Application::CreateBoid() {
	if (!m_pScene) {
		return nullptr;
	}

	Entity* pBoid = new Entity();
	// Create transform
	TransformComponent* pTransform = new TransformComponent(pBoid);
	// The absolute value for the maximum spawn distance.
	const int absoluteXDistance = m_pScene->GetOctTree().GetBoundary().GetDimensions().x;
	const int absoluteYDistance = m_pScene->GetOctTree().GetBoundary().GetDimensions().y;
	const int absoluteZDistance = m_pScene->GetOctTree().GetBoundary().GetDimensions().z;
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
		&m_pScene->GetOctTree());
	pBoid->AddComponent(COMPONENT_TYPE_COLLIDER, pCollider);
	// create brain i.e. AI controller
	BrainComponent* pBrain = new BrainComponent(pBoid, m_pScene);
	pBoid->AddComponent(COMPONENT_TYPE_BRAIN, static_cast<Component*>(pBrain));
	pBoid->SetTag("Boid");
	return pBoid;
}

Entity* Application::CreateObstacle(glm::vec3 a_spawnPosition) {
	if (!m_pScene) {
		return nullptr;
	}

	Entity* pObstacle = new Entity();
	// Add a new transform component.
	TransformComponent* obstaclesTransform = new TransformComponent(pObstacle);
	obstaclesTransform->SetMatrixRow(TransformComponent::MATRIX_ROW_POSITION_VECTOR, a_spawnPosition);
	pObstacle->AddComponent(COMPONENT_TYPE_TRANSFORM, static_cast<Component*>(obstaclesTransform));
	// Create a model to visualise the obstacle.
	ModelComponent* pModel = new ModelComponent(pObstacle);
	pModel->LoadModel("Resources/Models/Low_poly_UFO/Low_poly_UFO.obj");
	const float scaleScalar = 0.01f;
	pModel->SetScale(glm::vec3(scaleScalar));
	pObstacle->AddComponent(COMPONENT_TYPE_MODEL, static_cast<Component*>(pModel));
	ColliderComponent* pCollider = new ColliderComponent(pObstacle, &m_pScene->GetOctTree());
	const float dimensionsScale = 2.0f;
	pCollider->SetDimensions(dimensionsScale);
	pObstacle->AddComponent(COMPONENT_TYPE_COLLIDER, pCollider);
	pObstacle->SetTag("Obstacle");
	m_pScene->AddEntity(pObstacle);
	m_bSpawnedObstacle = true;
}

void Application::SetBoidCount(unsigned int a_uiBoidCount) {
	m_uiBoidCount = a_uiBoidCount;
	std::string boidTag = "Boid";
	unsigned int curentBoidCount = m_pScene->GetEntityCount(boidTag);

	if (m_uiBoidCount > curentBoidCount) {
		m_pScene->AddEntities(CreateBoid(), m_uiBoidCount - curentBoidCount);
	} else if (m_uiBoidCount < curentBoidCount) {
		m_pScene->DestroyEntitiesWithTag(boidTag, curentBoidCount - m_uiBoidCount);
	}
}

void Application::SetPauseState(bool a_bPaused) {
	m_bPaused = a_bPaused;
}

void Application::SetUpdateOnceState(bool a_bUpdateOnce) {
	m_bUpdateOnce = a_bUpdateOnce;
}
