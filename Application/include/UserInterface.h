//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/05/2021
//////////////////////////////

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

class Application;

class UserInterface
{
public:
	UserInterface(Application* a_pApplication) : m_pApplication(a_pApplication)
	{}
	~UserInterface()
	{}

	void Draw();

private:
	Application* m_pApplication;
};

#endif // !USER_INTERFACE_H
