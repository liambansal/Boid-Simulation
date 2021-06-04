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

void UserInterface::Draw()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();
	const float xPosition = 1.0f;
	const float heightScale = 0.75;
	const ImVec2 windowPosition = ImVec2(xPosition, io.DisplaySize.y * heightScale);
	ImGui::SetNextWindowPos(windowPosition, ImGuiCond_Always);

	// Setup sliders
	if (ImGui::Begin("Slider Menu"))
	{
		io.MouseDrawCursor = true;
		int boidCount = (int)m_pApplication->GetBoidCount();
		ImGui::SliderInt("Boid Count", &boidCount, 0, m_pApplication->GetMaximumBoidCount());
		m_pApplication->SetBoidCount(boidCount);
		const float maximumForceMultiplier = 1.0f;
		float separationForce = BrainComponent::GetSeparationForce();
		ImGui::SliderFloat("Separation Force", &separationForce, 0.0f, maximumForceMultiplier);
		BrainComponent::SetSeparationForce(separationForce);
		float alignmentForce = BrainComponent::GetAlignmentForce();
		ImGui::SliderFloat("Alignment Force", &alignmentForce, 0.0f, maximumForceMultiplier);
		BrainComponent::SetAlignmentForce(alignmentForce);
		float cohesionForce = BrainComponent::GetCohesionForce();
		ImGui::SliderFloat("Cohesion Force", &cohesionForce, 0.0f, maximumForceMultiplier);
		BrainComponent::SetCohesionForce(cohesionForce);
		float wanderForce = BrainComponent::GetWanderForce();
		ImGui::SliderFloat("Wander Force", &wanderForce, 0.0f, maximumForceMultiplier);
		BrainComponent::SetWanderForce(wanderForce);
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}