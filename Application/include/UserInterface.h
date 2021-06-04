//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/05/2021
//////////////////////////////

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class Application;

// For creating an interactive user interface using Imgui.
class UserInterface
{
public:
	UserInterface(Application* a_pApplication) : m_pApplication(a_pApplication)
	{}
	~UserInterface()
	{}

	void Draw();

private:
	// Application for the UI to appear on top of.
	Application* m_pApplication;
};

#endif // !USER_INTERFACE_H
