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

/// <summary>
/// A simple rendering program that handles creating a window to display 3D models
/// and supports the use of an interactable camera to inspect the models.
/// This is intended to act as a singleton class.
/// </summary>
class Framework {
public:
	/// <summary>
	/// Sets up the program's dependency programs and some important member variables.
	/// </summary>
	/// <param name="a_windowName"> The name of the program's display window. </param>
	/// <param name="a_width"> The display window's measurement in pixels across its x-axis. </param>
	/// <param name="a_height"> The display window's measurement in pixels across its y-axis. </param>
	/// <param name="a_pVertexShader"> The program's shader for manipulating vertices. </param>
	/// <param name="a_pFragmentShader"> The program's shader for colouring pixels. </param>
	/// <returns></returns>
	bool Initialize(const char* a_windowName,
		const int a_width,
		const int a_height,
		const char* a_pVertexShader,
		const char* a_pFragmentShader);
	/// <summary>
	/// Updates the program's logic.
	/// Always and only call once per frame.
	/// </summary>
	void Update();
	/// <summary>
	/// Uses the program's shader to draw a specified model on-screen.
	/// </summary>
	/// <param name="a_pModel"> The 3D model to draw on-screen. </param>
	void Draw(Model* a_pModel);
	void Draw();
	void Destory();

	/// <summary>
	/// Queries whether input for any keys has been registered this frame and handles the response.
	/// </summary>
	/// <param name="window"> The window that received the input event. </param>
	void ProcessInput(GLFWwindow* window);
	/// <summary>
	/// Callback for setting the program's window size.
	/// </summary>
	/// <param name="window"> The window that received the callback event. </param>
	/// <param name="width"> The new width of the frame buffer in pixels. </param>
	/// <param name="height"> The new height of the frame buffer in pixels. </param>
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	/// <summary>
	/// Callback for tracking the cursor's position as it moves.
	/// </summary>
	/// <param name="window"> The window that received the callback event. </param>
	/// <param name="xpos"> The cursor's current x coordinate. </param>
	/// <param name="ypos"> The cursor's current y coordinate. </param>
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	/// <summary>
	/// Callback for when the mouse's scroll wheel is used.
	/// </summary>
	/// <param name="window"> The window that received the callback event. </param>
	/// <param name="xoffset"> The scroll offset along the x-axis. </param>
	/// <param name="yoffset"> The scroll offset along the y-axis. </param>
	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	/// <summary>
	/// Returns the class' singleton instance.
	/// </summary>
	/// <returns> An instance of this class. </returns>
	static Framework* GetInstance();
	/// <summary>
	/// Returns the program's display window.
	/// </summary>
	/// <returns> A pointer to the program's display window. </returns>
	inline GLFWwindow* GetWindow() const;
	/// <summary>
	/// Returns the program's camera.
	/// </summary>
	/// <returns> A pointer to the program's camera. </returns>
	inline Camera* GetCamera() const;
	inline const float GetDeltaTime() const;
	/// <summary>
	/// Returns the program's shader.
	/// </summary>
	/// <returns> A pointer to the program's shader. </returns>
	inline const Shader* GetShader() const;
	inline const unsigned int GetScreenWidth() const;
	inline const unsigned int GetScreenHeight() const;

private:
	Framework();
	Framework(const Framework& a_rFramework);
	Framework& operator=(const Framework&);

	const unsigned int mc_uiScreenWidth;
	const unsigned int mc_uiScreenHeight;
	/// <summary>
	/// The cursor's x-coordinate on the previous frame.
	/// </summary>
	float m_fLastX;
	/// <summary>
	/// The cursor's y-coordinate on the previous frame.
	/// </summary>
	float m_fLastY;
	float m_fDeltaTime;
	/// <summary>
	/// The previous frame's timestamp.
	/// </summary>
	float m_fLastFrame;
	/// <summary>
	/// True during the program's first frame of execution while the previous-frame cursor's coordinates haven't been set.
	/// </summary>
	bool m_bFirstMouse;
	/// <summary>
	/// A pointer to the class' singleton instance.
	/// </summary>
	static Framework* ms_pInstance;
	/// <summary>
	/// A pointer to the program's display window.
	/// </summary>
	GLFWwindow* m_pWindow;
	/// <summary>
	/// A pointer to the program's user-interactable camera.
	/// </summary>
	Camera* m_pCamera;
	/// <summary>
	/// A shader for rendering textured 3D models.
	/// </summary>
	Shader* m_pModelShader;
	/// <summary>
	/// A shader for rendering lines in a 3D space.
	/// </summary>
	Shader* m_pLineShader;
};

GLFWwindow* Framework::GetWindow() const {
	return m_pWindow;
}

Camera* Framework::GetCamera() const {
	return m_pCamera;
}

const float Framework::GetDeltaTime() const {
	return m_fDeltaTime;
}

const Shader* Framework::GetShader() const {
	return m_pModelShader;
}

const unsigned int Framework::GetScreenWidth() const {
	return mc_uiScreenWidth;
}

const unsigned int Framework::GetScreenHeight() const {
	return mc_uiScreenHeight;
}

#endif // FRAMEWORK_H.
