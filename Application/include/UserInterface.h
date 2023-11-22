//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/05/2021
//////////////////////////////

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

// Forward declarations.
class Application;

/// <summary>
/// Draws a user interface for the application.
/// </summary>
class UserInterface
{
public:
	UserInterface(Application* a_pApplication) : m_pApplication(a_pApplication)
	{}
	~UserInterface()
	{}

	/// <summary>
	/// Draws all of the applications UI elements.
	/// </summary>
	void Draw();
	/// <summary>
	/// Draws the sliders that control the boids' movement forces.
	/// </summary>
	void DrawBoidsBehaviouralSliders();

private:
	/// <summary>
	/// The application instance that the UI is associated with.
	/// </summary>
	Application* m_pApplication;
};

#endif // !USER_INTERFACE_H
