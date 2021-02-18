//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

#include "Framework.h" // File's header.
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "LearnOpenGL/camera.h"
#include "LearnOpenGL/model.h"
#include "LearnOpenGL/shader.h"

Framework* Framework::ms_pInstance = nullptr;

Framework::Framework() : mc_uiScreenWidth(1280),
	mc_uiScreenHeight(800),
	m_fLastX(mc_uiScreenWidth * 0.5f),
	m_fLastY(mc_uiScreenHeight * 0.5f),
	m_fDeltaTime(0.0f),
	m_fLastFrame(0.0f),
	m_bFirstMouse(true),
	m_pWindow(nullptr),
	m_pCamera(nullptr),
	m_pModel(nullptr),
	m_pShader(nullptr)
{}

bool Framework::Initialize(const char* a_windowName,
	const int a_width,
	const int a_height,
	const char* a_pVertexShader,
	const char* a_pFragmentShader,
	const char* a_pModelFilepath)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		return false;
	}

	m_pWindow = glfwCreateWindow(a_width,
		a_height,
		a_windowName,
		nullptr, nullptr);

	if (!m_pWindow)
	{
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

	// Initialize GLAD.
	if (!gladLoadGL())
	{
		std::cout << "Error: Failed to initialize GLAD.\n";
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	m_pCamera = new Camera(glm::vec3(0.0f, 0.0f, 0.3f));
	m_pShader = new Shader(a_pVertexShader, a_pFragmentShader);
	m_pModel = new Model(a_pModelFilepath);

	// Configure global opengl state.
	glEnable(GL_DEPTH_TEST);
	const int major = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MAJOR);
	const int minor = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_VERSION_MINOR);
	const int revision = glfwGetWindowAttrib(m_pWindow, GLFW_CONTEXT_REVISION);
	std::cout << "OpenGl Version Supported: " << major << "." << minor << "." << revision << std::endl;
	return true;
}

void Framework::Update()
{
	do
	{
		glClearColor(0.5f, 0.5f, 0.5f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Per-frame time logic.
		float currentFrame = glfwGetTime();
		m_fDeltaTime = currentFrame - m_fLastFrame;
		m_fLastFrame = currentFrame;
		
		// Input.
		ProcessInput(m_pWindow);

		if (m_pCamera == nullptr || m_pModel == nullptr || m_pShader == nullptr)
		{
			// Early out if any pointers are null.
			return;
		}

		// Don't forget to enable shader before setting uniforms.
		m_pShader->use();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(m_pCamera->Zoom),
			(float)mc_uiScreenWidth / (float)mc_uiScreenHeight,
			0.1f,
			100.0f);
		glm::mat4 view = m_pCamera->GetViewMatrix();
		m_pShader->setMat4("projection", projection);
		m_pShader->setMat4("view", view);

		// Render the loaded model.
		glm::mat4 model = glm::mat4(1.0f);
		// Translate it down so it's at the center of the scene.
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		// It's a bit too big for our scene, so scale it down.
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		m_pShader->setMat4("model", model);
		m_pModel->Draw(*m_pShader);

		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
		// Return whether to close or not.
	} while (glfwWindowShouldClose(m_pWindow) == 0);
}

void Framework::Destory()
{
	delete m_pCamera;
	m_pCamera = nullptr;
	delete m_pShader;
	m_pShader = nullptr;
	delete m_pModel;
	m_pModel = nullptr;
	// Destroy the scene window.
	glfwDestroyWindow(m_pWindow);
	// Clear all previously allocated GLFW resources.
	glfwTerminate();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void Framework::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_pCamera->ProcessKeyboard(FORWARD, m_fDeltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_pCamera->ProcessKeyboard(BACKWARD, m_fDeltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_pCamera->ProcessKeyboard(LEFT, m_fDeltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_pCamera->ProcessKeyboard(RIGHT, m_fDeltaTime);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void Framework::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void Framework::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Framework* pFramework = Framework::GetInstance();

	if (!pFramework)
	{
		// Return early.
		return;
	}

	if (pFramework->m_bFirstMouse)
	{
		pFramework->m_fLastX = xpos;
		pFramework->m_fLastY = ypos;
		pFramework->m_bFirstMouse = false;
	}

	float xoffset = xpos - pFramework->m_fLastX;
	float yoffset = pFramework->m_fLastY - ypos; // reversed since y-coordinates go from bottom to top
	pFramework->m_fLastX = xpos;
	pFramework->m_fLastY = ypos;

	if (pFramework->m_pCamera)
	{
		pFramework->m_pCamera->ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void Framework::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Framework* pFramework = Framework::GetInstance();

	if (!pFramework || !pFramework->m_pCamera)
	{
		// Return early.
		return;
	}

	pFramework->m_pCamera->ProcessMouseScroll(yoffset);
}

Framework* Framework::GetInstance()
{
	if (ms_pInstance == nullptr)
	{
		ms_pInstance = new Framework();
	}

	return ms_pInstance;
}