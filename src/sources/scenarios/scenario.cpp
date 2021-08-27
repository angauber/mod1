#include "scenarios/scenario.hpp"

Scenario::Scenario(std::shared_ptr<Grid> grid) : grid {grid} {};

void	Scenario::setupScenario() const
{
	for (std::size_t i = 0; i < this->grid->size; i++) {
		for (std::size_t j = 0; j < this->grid->size; j++) {
			this->grid->get(i, j)->clear();
		}
	}
}

void	Scenario::updateScenario() const
{}

void	Scenario::drawGUI()
{
	if (ImGui::Button("Reset")) {
		this->grid->reset();
		this->setupScenario();
	}
}
