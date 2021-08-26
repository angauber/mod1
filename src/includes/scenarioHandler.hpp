#ifndef _SCENARIO_HANDLER_HPP_
#define _SCENARIO_HANDLER_HPP_

#include <map>
#include <string>
#include <memory>
#include <Magnum/Timeline.h>

#include "waterSimulation.hpp"
#include "scenario.hpp"
#include "waterRiseScenario.hpp"
#include "waveScenario.hpp"
#include "rainScenario.hpp"
#include "waterSpreadScenario.hpp"

using namespace Magnum;

class ScenarioHandler : public WaterSimulation
{
	protected:
									ScenarioHandler();
		std::string					getScenarioKey() const;
		std::shared_ptr<Scenario>	getScenario();
		void						setScenario(const std::string &key);
		void						resetScenario();
		void						drawScenarioGUI();
		void						updateScenario();

		std::map<std::string, std::shared_ptr<Scenario>>	scenarios;
	private:
		Timeline											timeline;
		float												timeToUpdate {0.0f};
		std::string											scenario {"rise"};
};

#endif
