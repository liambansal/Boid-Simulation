//////////////////////////////
// Author: Liam Bansal
// Date Created: 17/05/2021
//////////////////////////////

#ifndef APPLICATION_H
#define APPLICATION_H

// Header includes.
#include "glm/ext.hpp"

// Forward declarations.
class Entity;
class Framework;
class Scene;
class UserInterface;
class TransformComponent;

/// <summary>
/// A 'top level' class for simulating boids which everything else will work from.
/// </summary>
class Application
{
public:
	Application();
	~Application();

	/// <summary>
	/// Keeps the application open and operating.
	/// </summary>
	void Run();
	/// <summary>
	/// Checks if the applicaion should stop running due to user input or a failure to correctly initialise.
	/// </summary>
	/// <returns> True if the application should stop runnning, otherwise false. </returns>
	bool CloseApplication() const;
	/// <summary>
	/// Updates the state of the application.
	/// </summary>
	void Update();
	/// <summary>
	/// Queries and responds to input captured by GLFW.
	/// </summary>
	void ProcessInput();
	/// <summary>
	/// Renders the application's contents within its target window.
	/// </summary>
	void Draw();
	/// <summary>
	/// Creates a new boid entity.
	/// </summary>
	/// <returns> A pointer to the newly created boid entity. </returns>
	Entity* CreateBoid();
	/// <summary>
	/// Creates a new obstacle entity for boids to avoid.
	/// </summary>
	/// <param name="a_pTransform"> The position to spawn the obstacle at. </param>
	/// <returns> A pointer to the newly created obstacle entity. </returns>
	Entity* CreateObstacle(glm::vec3 a_spawnPosition);

	/// <summary>
	/// Sets how many boids should be simulated by the application.
	/// </summary>
	/// <param name="a_uiBoidCount"> The desired number of boids to simulate. </param>
	void SetBoidCount(unsigned int a_uiBoidCount);
	
	inline unsigned int GetBoidCount() const;
	inline unsigned int GetMaximumBoidCount() const;

private:
	/// <summary>
	/// The current number of boids that exist within the application.
	/// </summary>
	unsigned int m_uiBoidCount;
	const unsigned int mc_uiMaximumBoidCount;
	/// <summary>
	/// The distance between the application's camera and virtual world based cursor.
	/// </summary>
	float m_fMarkerZOffset;
	const float mc_fMimimumMarkerZOffset;
	const float mc_fMaximumMarkerZOffset;
	bool m_bFrameworkInitialised;
	/// <summary>
	/// True if the user has spawned an obstacle for the boids to avoid collisions with.
	/// </summary>
	bool m_bSpawnedObstacle;
	/// <summary>
	/// A pointer to the program that's responsible for drawing the majority of the application.
	/// </summary>
	Framework* m_pFramework;
	/// <summary>
	/// A pointer to a program that handles creating, updating, drawing, and destroying entities.
	/// </summary>
	Scene* m_pScene;
	UserInterface* m_pUserInterface;
	/// <summary>
	/// A 3D world space cursor.
	/// </summary>
	Entity* m_pWorldCursor;
};

unsigned int Application::GetBoidCount() const
{
	return m_uiBoidCount;
}

unsigned int Application::GetMaximumBoidCount() const
{
	return mc_uiMaximumBoidCount;
}

#endif // !APPLICATION_H.
