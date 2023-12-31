//////////////////////////////
// Author: Liam Bansal
// Date Created: 18/05/2021
//////////////////////////////

// File's header.
#include "UserInterface.h"
#include "Application.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "BrainComponent.h"
#include "ColliderComponent.h"
#include "Entity.h"

void UserInterface::Draw() const {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	const float xPosition = 1.0f;
	const float yPosition = io.DisplaySize.y * 0.6f;
	const ImVec2 windowPosition = ImVec2(xPosition, yPosition);
	ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always);

	if (ImGui::Begin("Application User Interface")) {
		io.MouseDrawCursor = true;
		DrawPhysicsControls();
		// Add an empty line to separate the different UI sections.
		ImGui::NewLine();
		DrawBoidControls();
		ImGui::NewLine();
		DrawTimeControls();
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UserInterface::DrawBoidControls() const {
	if (!m_pApplication) {
		return;
	}

	// UI section header.
	ImGui::Text("Boid Forces");

	// Slider for controlling the number of boids.
	int boidCount = (int)m_pApplication->GetBoidCount();
	ImGui::SliderInt("Boid Count", &boidCount, 0, m_pApplication->GetMaximumBoidCount());
	m_pApplication->SetBoidCount(boidCount);

	const float minimumForceMultiplier = 0.0f;
	const float maximumForceMultiplier = 1.0f;

	// Slider for controlling the boids' separation force.
	float separationForce = BrainComponent::GetSeparationForce();
	ImGui::SliderFloat("Separation Force", &separationForce, minimumForceMultiplier, maximumForceMultiplier);
	BrainComponent::SetSeparationForce(separationForce);

	// Slider for controlling the boids' alignment force.
	float alignmentForce = BrainComponent::GetAlignmentForce();
	ImGui::SliderFloat("Alignment Force", &alignmentForce, minimumForceMultiplier, maximumForceMultiplier);
	BrainComponent::SetAlignmentForce(alignmentForce);

	// Slider for controlling the boids' cohesion force.
	float cohesionForce = BrainComponent::GetCohesionForce();
	ImGui::SliderFloat("Cohesion Force", &cohesionForce, minimumForceMultiplier, maximumForceMultiplier);
	BrainComponent::SetCohesionForce(cohesionForce);

	// Slider for controlling the boids' wander force.
	float wanderForce = BrainComponent::GetWanderForce();
	ImGui::SliderFloat("Wander Force", &wanderForce, minimumForceMultiplier, maximumForceMultiplier);
	BrainComponent::SetWanderForce(wanderForce);

	bool drawColliders = ColliderComponent::GetColliderDrawState();
	ImGui::Checkbox("Draw Colliders", &drawColliders);
	ColliderComponent::SetColliderDrawState(drawColliders);
}

void UserInterface::DrawPhysicsControls() const {
	// UI section header.
	ImGui::Text("Physics");
	bool collisionsOn = Entity::GetCollisionsState();
	ImGui::Checkbox("Boid Collisions On", &collisionsOn);
	Entity::SetCollisionsState(collisionsOn);
}

void UserInterface::DrawTimeControls() const {
	if (!m_pApplication) {
		return;
	}

	// UI section header.
	ImGui::Text("Time");

	bool applicationPaused = m_pApplication->GetPauseState();
	ImGui::Checkbox("Paused", &applicationPaused);
	m_pApplication->SetPauseState(applicationPaused);

	bool updateOnce = ImGui::Button("Update Once", ImVec2(100, 20));
	m_pApplication->SetUpdateOnceState(updateOnce);
}
