#include "waveScenario.hpp"

float	WaveScenario::getTimeStep() const
{
	return 0.0f;
}

void	WaveScenario::setupScenario() const
{
	for (std::size_t i = 0; i < this->grid->size; i++) {
		for (std::size_t j = 0; j < this->grid->size; j++) {
			if (i < this->grid->size / 20) {
				this->grid->get(i, j)->waterDepth = 0.3f;
				this->grid->get(i, j)->downPipe = -0.1f;
			}
		}
	}
}
