//////////////////////////////
// Author: Liam Bansal.
// Date Created: 21/01/2021.
//////////////////////////////

#include "Application.h"
#include "Framework.h"

int main() {
	Framework* pFramework = Framework::GetInstance();

	if (pFramework) {
		const char* applicationName = "Boids Simulation";
		// Initialize the rendering framework prior to creating the application so glad is already setup for use by 
		// the application's boundary.
		pFramework->Initialize(applicationName,
			pFramework->GetScreenWidth(),
			pFramework->GetScreenHeight(),
			"Resources/Shaders/model_loading.vs",
			"Resources/Shaders/model_loading.fs",
			"Resources/Shaders/lineRenderer.vs",
			"Resources/Shaders/lineRenderer.fs");
		Application application;
		application.Run();
	}

	return 0;
}