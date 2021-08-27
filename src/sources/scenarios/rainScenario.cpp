#include "scenarios/rainScenario.hpp"

float	RainScenario::getTimeStep() const
{
	return 0.05f;
}

void	RainScenario::updateScenario() const
{
	for (std::size_t i = 0; i < this->grid->size; i++) {
		for (std::size_t j = 0; j < this->grid->size; j++) {
			if (((i+j) % 2 == 0) && this->grid->get(i, j)->surfaceHeight() < 1.0f) {
				this->grid->get(i, j)->waterDepth += rise * 0.00001f;
			}
		}
	}

}

void	RainScenario::drawGUI()
{
	Scenario::drawGUI();

	ImGui::SliderFloat("Rain flow", &this->rise, 0.0f, 50.0f);
}
