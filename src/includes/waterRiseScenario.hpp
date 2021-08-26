#ifndef _WATER_RISE_SCENARIO_HPP_
#define _WATER_RISE_SCENARIO_HPP_

#include "scenario.hpp"

#include <Magnum/Magnum.h>

class WaterRiseScenario : public Scenario {
	public:
		using	Scenario::Scenario;
		float	getTimeStep() const override;
		void	updateScenario() const override;
		void	drawGUI() override;
	private:
		float	rise {40.0f};
};

#endif
