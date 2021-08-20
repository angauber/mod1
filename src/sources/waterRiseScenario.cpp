#include "waterRiseScenario.hpp"

float	WaterRiseScenario::getTimeStep() const
{
	return 0.05f;
}

void	WaterRiseScenario::setupScenario(SimulationGrid &grid, std::size_t size) const
{}

void	WaterRiseScenario::updateScenario(SimulationGrid &grid, std::size_t size) const
{
	float threshold {0.0f};
	float rise {0.005f};

	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			if (grid[i][j].isWet() && (grid[i][j].surfaceHeight() > threshold)) {
				threshold = grid[i][j].surfaceHeight();
			}
		}
	}

	threshold += rise;

	if (threshold < 1.0f) {
		for (std::size_t i = 0; i < size; i++) {
			for (std::size_t j = 0; j < size; j++) {
				if (grid[i][j].surfaceHeight() < threshold) {
					grid[i][j].waterDepth = threshold - grid[i][j].terrainHeight;
				}
			}
		}
	}
}
