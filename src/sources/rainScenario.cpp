#include "rainScenario.hpp"

float	RainScenario::getTimeStep() const
{
	return 0.05f;
}

void	RainScenario::setupScenario(SimulationGrid &grid, std::size_t size) const
{}

void	RainScenario::updateScenario(SimulationGrid &grid, std::size_t size) const
{
	float rise {0.0001f};

	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			if (((i+j) % 2 == 0) && grid[i][j].surfaceHeight() < 1.0f) {
				grid[i][j].waterDepth += rise;
			}
		}
	}

}
