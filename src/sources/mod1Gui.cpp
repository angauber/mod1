#include "mod1.hpp"

void	Mod1::drawGUI()
{
	this->imgui.newFrame();

	if(ImGui::GetIO().WantTextInput && !this->isTextInputActive())
	    this->startTextInput();
	else if(!ImGui::GetIO().WantTextInput && this->isTextInputActive())
		this->stopTextInput();

//	ImGui::ShowDemoWindow();

	{
		ImGui::Begin("Mod1");

		ImGui::Text("%d FPS", this->fps);

		if (ImGui::TreeNode("Scenario")) {
			
			if (ImGui::BeginCombo("Scenario", this->getScenarioKey().c_str())) {
				for (const auto &key : std::views::keys(this->scenarios)) {
        	        const bool isSelected = key == this->getScenarioKey();

        	        if (ImGui::Selectable(key.c_str(), isSelected)) this->setScenario(key);

                	if (isSelected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}


			this->drawScenarioGUI();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Simulation")) {
			static std::string label {"Pause"};
			
			if (ImGui::Button(label.c_str())) {
				this->running = !this->running;
				label = this->running ? "Pause" : "Play";
			}

	        ImGui::SliderFloat("Viscosity", &this->viscosity, 0.5f, 10.0f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Colors")) {
	        ImGui::Text("Water Color");

			ImGui::ColorEdit4("Shallow", this->waterStartColor, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit4("Deep", this->waterEndColor, ImGuiColorEditFlags_NoInputs);

	        ImGui::Text("Terrain Color");

			if (ImGui::ColorEdit3("Low altitude", this->terrainStartColor, ImGuiColorEditFlags_NoInputs) ||
				ImGui::ColorEdit3("High altitude", this->terrainEndColor, ImGuiColorEditFlags_NoInputs)) {
				this->terrainMesh = this->createTerrainMesh();
			}
			
			ImGui::TreePop();
		}

		ImGui::End();
	}
	
	this->imgui.updateApplicationCursor(*this);

	/* Set appropriate states. If you only draw ImGui, it is sufficient to
	    just enable blending and scissor test in the constructor. */
	 GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
	 GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
	 GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

	this->imgui.drawFrame();

	 /* Reset state. Only needed if you want to draw something else with
		different state after. */
	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
	GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
	GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
}
