#include "waterRiseScenario.hpp"

float	WaterRiseScenario::getTimeStep() const
{
	return 0.05f;
}

void	WaterRiseScenario::updateScenario() const
{
	float threshold {0.0f};
	float rise {0.005f};
	Cell *cell;

	for (std::size_t i = 0; i < this->grid->size; i++) {
		for (std::size_t j = 0; j < this->grid->size; j++) {
			cell = this->grid->get(i, j);

			if (cell->isWet() && (cell->surfaceHeight() > threshold)) {
				threshold = cell->surfaceHeight();
			}
		}
	}

	threshold += rise;

	if (threshold < 1.0f) {
		for (std::size_t i = 0; i < this->grid->size; i++) {
			for (std::size_t j = 0; j < this->grid->size; j++) {
				if (cell->surfaceHeight() < threshold) {
					cell->waterDepth = threshold - cell->terrainHeight;
				}
			}
		}
	}
}
