#include "waveScenario.hpp"

float	WaveScenario::getTimeStep() const
{
	return 0.0f;
}

void	WaveScenario::setupScenario(SimulationGrid &grid, std::size_t size) const
{
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			if (i < size / 10) {
				grid[i][j].waterDepth = 0.2f;
			}
		}
	}
}

void	WaveScenario::updateScenario(SimulationGrid &grid, std::size_t size) const
{}
