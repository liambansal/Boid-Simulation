//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

// Forward declarations.
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
		const char* a_pFragmentShader);
	void Update();
	void Draw(Model* a_pModel);
	void Destory();

	void ProcessInput(GLFWwindow* window);
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	static Framework* GetInstance();
	inline GLFWwindow* GetWindow() const;
	inline Camera* GetCamera() const;
	inline const float GetDeltaTime() const;
	inline const Shader* GetShader() const;
	inline const unsigned int GetScreenWidth() const;
	inline const unsigned int GetScreenHeight() const;

private:
	Framework();
	Framework(const Framework& a_rFramework);
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
};

GLFWwindow* Framework::GetWindow() const
{
	return m_pWindow;
}

Camera* Framework::GetCamera() const
{
	return m_pCamera;
}

const float Framework::GetDeltaTime() const
{
	return m_fDeltaTime;
}

const Shader* Framework::GetShader() const
{
	return m_pShader;
}

const unsigned int Framework::GetScreenWidth() const
{
	return mc_uiScreenWidth;
}

const unsigned int Framework::GetScreenHeight() const
{
	return mc_uiScreenHeight;
}

#endif // FRAMEWORK_H.
