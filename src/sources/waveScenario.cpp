#include "waveScenario.hpp"

float	WaveScenario::getTimeStep() const
{
	return 0.0f;
}

void	WaveScenario::setupScenario() const
{
	float limit {static_cast<float> (this->grid->size) / 10};
	Cell *cell;

	for (std::size_t i = 0; i < this->grid->size; i++) {
		for (std::size_t j = 0; j < this->grid->size; j++) {
			if (i < limit) {
				cell = this->grid->get(i, j);

				cell->waterDepth = Math::lerp(0.10f, 0.25f, static_cast<float> (i) / limit) - cell->terrainHeight;
				cell->downPipe = -0.1f;
			}
		}
	}
}
