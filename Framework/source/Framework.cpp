#include "Framework.h" //FIle's header.
#include <iostream>
#include "glm.hpp"
#include "GTC/matrix_transform.hpp"
#include "GTC/type_ptr.hpp"
#include "stb_image.h"

Framework::Framework() : m_pWindow(nullptr)
{}

Framework::~Framework()
{}

bool Framework::Initialize(const char* a_windowName,
	const int a_width,
	const int a_height)
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
		glfwTerminate();
		return false;
	}

	// Make new window current rendering context.
	glfwMakeContextCurrent(m_pWindow);

	// Initialize GLAD.
	if (!gladLoadGL())
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
		return false;
	}

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
		glClearColor(0.6f, 0.2f, 0.4f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}
	while (glfwWindowShouldClose(m_pWindow) == 0);
}

void Framework::Destory()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}
