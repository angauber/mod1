#include "scenarioHandler.hpp"

ScenarioHandler::ScenarioHandler()
{
	this->scenarios["rise"] = std::make_shared<WaterRiseScenario> (this->grid);
	this->scenarios["spread"] = std::make_shared<WaterSpreadScenario> (this->grid);
	this->scenarios["wave"] = std::make_shared<WaveScenario> (this->grid);
	this->scenarios["rain"] = std::make_shared<RainScenario> (this->grid);

	this->timeline.start();
}

std::shared_ptr<Scenario>	ScenarioHandler::getScenario()
{
	if (this->scenarios.contains(this->scenario)) {
		return this->scenarios[this->scenario];
	}

	throw std::runtime_error(std::string {"Scenario: "} + this->scenario + std::string {"does not exists"});
}

std::string	ScenarioHandler::getScenarioKey() const
{
	return this->scenario;
}

void	ScenarioHandler::setScenario(const std::string &key)
{
	this->scenario = key;

	this->resetScenario();
}

void	ScenarioHandler::resetScenario()
{
	this->grid->reset();

	this->getScenario()->setupScenario();
}

void	ScenarioHandler::drawScenarioGUI()
{
	this->getScenario()->drawGUI();
}

void	ScenarioHandler::updateScenario()
{
	auto scenario = this->getScenario();

	if (scenario->getTimeStep() > 0.0f) {
		if (this->timeToUpdate > scenario->getTimeStep()) {
			scenario->updateScenario();
			this->timeToUpdate = 0.0f;
		}
		this->timeToUpdate += this->timeline.previousFrameDuration();
	}

	this->timeline.nextFrame();

}
