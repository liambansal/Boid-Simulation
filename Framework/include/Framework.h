#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Framework
{
public:
	Framework();
	~Framework();

	bool Initialize(const char* a_windowName,
		const int a_width,
		const int a_height);
	void Update();
	void Destory();

private:
	GLFWwindow* m_pWindow;
};

#endif // FRAMEWORK_H.
