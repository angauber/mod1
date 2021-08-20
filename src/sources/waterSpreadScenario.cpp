#include "waterSpreadScenario.hpp"

float	WaterSpreadScenario::getTimeStep() const
{
	return 0.0f;
}

void	WaterSpreadScenario::setupScenario(SimulationGrid &grid, std::size_t size) const
{
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			if (i > 40 && i < 60 && j > 40 && j < 60) {
				grid[i][j].waterDepth = 0.2f;
			}
		}
	}
}


void	WaterSpreadScenario::updateScenario(SimulationGrid &grid, std::size_t size) const
{}
