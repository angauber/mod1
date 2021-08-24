#include "scenario.hpp"

void	Scenario::setupScenario(SimulationGrid &grid, std::size_t size) const
{
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			grid[i][j].clear();
		}
	}
}

void	Scenario::updateScenario(SimulationGrid &grid, std::size_t size) const
{}

void	Scenario::drawGUI(SimulationGrid &grid, std::size_t size)
{
	if (ImGui::Button("Reset")) {
		this->setupScenario(grid, size);
	}
}
