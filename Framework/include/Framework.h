//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

struct GLFWwindow;
class Camera;
class Model;
class Shader;

// Singleton class.
class Framework
{
public:
	bool Initialize(const char* a_windowName,
		const int a_width,
		const int a_height,
		const char* a_pVertexShader,
		const char* a_pFragmentShader,
		const char* a_pModelFilepath);
	void Update();
	void Destory();

	void ProcessInput(GLFWwindow* window);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static Framework* GetInstance();

private:
	Framework();
	Framework(const Framework& a_framework);
	Framework& operator=(const Framework&);
	
	const unsigned int mc_uiScreenWidth;
	const unsigned int mc_uiScreenHeight;
	float m_fLastX;
	float m_fLastY;
	float m_fDeltaTime;
	float m_fLastFrame;
	bool m_bFirstMouse;

	static Framework* ms_pInstance;
	GLFWwindow* m_pWindow;
	Camera* m_pCamera;
	Shader* m_pShader;
	Model* m_pModel;
};

#endif // FRAMEWORK_H.
