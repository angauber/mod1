#include "waterSpreadScenario.hpp"

float	WaterSpreadScenario::getTimeStep() const
{
	return 0.0f;
}

void	WaterSpreadScenario::setupScenario() const
{
	for (std::size_t i = 0; i < this->grid->size; i++) {
		for (std::size_t j = 0; j < this->grid->size; j++) {
			if (i > this->grid->size * 0.4f && i < this->grid->size * 0.6f  && j > this->grid->size * 0.4f && j < this->grid->size * 0.6f) {
				this->grid->get(i, j)->waterDepth = 0.3f;
			}
		}
	}
}
