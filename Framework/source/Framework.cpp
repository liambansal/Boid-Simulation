//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

#include "Framework.h" // File's header.
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include <iostream>
#include "LearnOpenGL/camera.h"
#include "LearnOpenGL/model.h"
#include "LearnOpenGL/shader.h"

Framework* Framework::ms_pInstance = nullptr;

Framework::Framework() : m_bInitialized(false),
	mc_uiScreenWidth(1280),
	mc_uiScreenHeight(800),
	m_fLastCursorXPosition(0),
	m_fLastCursorYPosition(0),
	m_fDeltaTime(0.0f),
	m_fTimeOfLastFrame(0.0f),
	m_bLastCursorCoordinatesSet(false),
	m_pWindow(nullptr),
	m_pCamera(new Camera(glm::vec3(0.0f, 0.0f, -20.0f))),
	m_pModelShader(nullptr),
	m_pLineShader(nullptr) {}

bool Framework::Initialize(const char* a_windowName,
	const int a_width,
	const int a_height,
	const char* a_pModelVertexShader,
	const char* a_pModelFragmentShader,
	const char* a_pLineVertexShader,
	const char* a_pLineFragmentShader) {
#pragma region GLFW Setup
	// Initialize GLFW.
	if (!glfwInit()) {
		return false;
	}

	m_pWindow = glfwCreateWindow(a_width,
		a_height,
		a_windowName,
		nullptr, nullptr);

	if (!m_pWindow) {
		std::cout << "Error: Failed to create GLFW window.\n";
		glfwTerminate();
		return false;
	}

	// Make new window current rendering context.
	glfwMakeContextCurrent(m_pWindow);
	glfwSetFramebufferSizeCallback(m_pWindow, FramebufferSizeCallback);
	glfwSetCursorPosCallback(m_pWindow, MouseCallback);
	glfwSetScrollCallback(m_pWindow, ScrollCallback);

	// Tell GLFW to capture our mouse.
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#pragma endregion

#pragma region GLAD Setup
	// Initialize GLAD.
	if (!gladLoadGL()) {
		std::cout << "Error: Failed to initialize GLAD.\n";
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
#pragma endregion

#pragma region ImGui setup
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 150";
	ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
#pragma endregion

	m_pModelShader = new Shader(a_pModelVertexShader, a_pModelFragmentShader);
	m_pLineShader = new Shader(a_pLineVertexShader, a_pLineFragmentShader);

	// Configure global opengl state.
	glEnable(GL_DEPTH_TEST);
	const int major = glfwGetWindowAttrib(m_pWindow,
		GLFW_CONTEXT_VERSION_MAJOR);
	const int minor = glfwGetWindowAttrib(m_pWindow,
		GLFW_CONTEXT_VERSION_MINOR);
	const int revision = glfwGetWindowAttrib(m_pWindow,
		GLFW_CONTEXT_REVISION);
	std::cout << "OpenGl Version Supported: " << major << "." << minor << "." << revision << std::endl;

	m_bInitialized = true;
	return true;
}

void Framework::Update() {
	// Per-frame time logic.
	float currentTime = glfwGetTime();
	m_fDeltaTime = currentTime - m_fTimeOfLastFrame;
	m_fTimeOfLastFrame = currentTime;
	ProcessInput(m_pWindow);
}

void Framework::DrawModel(Model* a_pModel) {
	if (!m_pModelShader || !m_pCamera || !a_pModel) {
		return;
	}

	// Don't forget to enable shader before setting uniforms.
	m_pModelShader->use();
	// view/projection transforms.
	m_pModelShader->setMat4("projection", m_pCamera->GetProjectionMatrix(mc_uiScreenWidth, mc_uiScreenHeight));
	m_pModelShader->setMat4("view", m_pCamera->GetViewMatrix());
	a_pModel->Draw(*m_pModelShader);
}

void Framework::UseLineShader() {
	if (!m_pLineShader || !m_pCamera) {
		return;
	}

	m_pLineShader->use();
	m_pLineShader->setMat4("projection", m_pCamera->GetProjectionMatrix(mc_uiScreenWidth, mc_uiScreenHeight));
	m_pLineShader->setMat4("view", m_pCamera->GetViewMatrix());
}

void Framework::Destory() {
	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_pModelShader;
	m_pModelShader = nullptr;
	delete m_pLineShader;
	m_pLineShader = nullptr;
	// Clean up imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	// Destroy the scene window.
	glfwDestroyWindow(m_pWindow);
	// Clear all previously allocated GLFW resources.
	glfwTerminate();
}

void Framework::ProcessInput(GLFWwindow* a_pWindow) {
	if (glfwGetKey(a_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(a_pWindow, true);
	}

	if (glfwGetKey(a_pWindow, GLFW_KEY_W) == GLFW_PRESS) {
		m_pCamera->ProcessKeyboard(FORWARD, m_fDeltaTime);
	}

	if (glfwGetKey(a_pWindow, GLFW_KEY_S) == GLFW_PRESS) {
		m_pCamera->ProcessKeyboard(BACKWARD, m_fDeltaTime);
	}

	if (glfwGetKey(a_pWindow, GLFW_KEY_A) == GLFW_PRESS) {
		m_pCamera->ProcessKeyboard(LEFT, m_fDeltaTime);
	}

	if (glfwGetKey(a_pWindow, GLFW_KEY_D) == GLFW_PRESS) {
		m_pCamera->ProcessKeyboard(RIGHT, m_fDeltaTime);
	}

	if (glfwGetKey(a_pWindow, GLFW_KEY_E) == GLFW_PRESS) {
		m_pCamera->ProcessKeyboard(UP, m_fDeltaTime);
	}

	if (glfwGetKey(a_pWindow, GLFW_KEY_Q) == GLFW_PRESS) {
		m_pCamera->ProcessKeyboard(DOWN, m_fDeltaTime);
	}

	if (glfwGetKey(a_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		m_pCamera->ProcessKeyboard(MOVE_FASTER, m_fDeltaTime);
	}
}

void Framework::FramebufferSizeCallback(GLFWwindow* a_pWindow, int a_windowWidth, int a_windowHeight) {
	// Make sure the view port matches the new window dimensions. The width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, a_windowWidth, a_windowHeight);
}

void Framework::MouseCallback(GLFWwindow* a_pWindow, double a_cursorsXPosition, double a_cursorsYPosition) {
	Framework* pFramework = Framework::GetInstance();

	if (!pFramework) {
		return;
	}

	if (!pFramework->m_bLastCursorCoordinatesSet) {
		// Any change in the cursor's position shouldn't be registered if the previous coordinates haven't been set.
		pFramework->m_fLastCursorXPosition = a_cursorsXPosition;
		pFramework->m_fLastCursorYPosition = a_cursorsYPosition;
		pFramework->m_bLastCursorCoordinatesSet = true;
	}

	float chagneInXPosition = a_cursorsXPosition - pFramework->m_fLastCursorXPosition;
	// Screen-space coordinates start from the top left corner of the window. 
	// Any change in position is added onto the camera's rotation. So positive numbers move up and negative numbers 
	// moves down.
	float changeInYPosition = pFramework->m_fLastCursorYPosition - a_cursorsYPosition;
	pFramework->m_fLastCursorXPosition = a_cursorsXPosition;
	pFramework->m_fLastCursorYPosition = a_cursorsYPosition;

	if (pFramework->m_pCamera && glfwGetMouseButton(a_pWindow, GLFW_MOUSE_BUTTON_RIGHT)) {
		pFramework->m_pCamera->ProcessMouseMovement(chagneInXPosition, changeInYPosition);
	}
}

void Framework::ScrollCallback(GLFWwindow* a_pWindow, double a_scrollXOffset, double a_scrollYOffset) {
	Framework* pFramework = Framework::GetInstance();

	if (!pFramework || !pFramework->m_pCamera) {
		return;
	}

	pFramework->m_pCamera->ProcessMouseScroll(a_scrollYOffset);
}

Framework* Framework::GetInstance() {
	if (ms_pInstance == nullptr) {
		ms_pInstance = new Framework();
	}

	return ms_pInstance;
}
