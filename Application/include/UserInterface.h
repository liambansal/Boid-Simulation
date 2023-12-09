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
class UserInterface {
public:
	UserInterface(Application* a_pApplication) : m_pApplication(a_pApplication) {}
	~UserInterface() {}

	/// <summary>
	/// Draws all of the applications UI elements.
	/// </summary>
	void Draw() const;
	/// <summary>
	/// Draws the sliders that control the boids' movement forces.
	/// </summary>
	void DrawBoidControls() const;
	/// <summary>
	/// Draws the UI elements that control the physical properties of the simulation.
	/// </summary>
	void DrawPhysicsControls() const;
	/// <summary>
	/// Draws the UI elements that control the application's pause/play states.
	/// </summary>
	void DrawTimeControls() const;

private:
	/// <summary>
	/// The application instance that the UI is associated with.
	/// </summary>
	Application* m_pApplication;
};

#endif // !USER_INTERFACE_H
