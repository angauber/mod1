#include "waterSpreadScenario.hpp"

float	WaterSpreadScenario::getTimeStep() const
{
	return 0.0f;
}

void	WaterSpreadScenario::setupScenario(SimulationGrid &grid, std::size_t size) const
{
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			if (i > size * 0.4f && i < size * 0.6f  && j > size * 0.4f && j < size * 0.6f) {
				grid[i][j].waterDepth = 0.3f;
			}
		}
	}
}


void	WaterSpreadScenario::updateScenario(SimulationGrid &grid, std::size_t size) const
{}
