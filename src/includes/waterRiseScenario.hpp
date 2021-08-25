#ifndef _WATER_RISE_SCENARIO_HPP_
#define _WATER_RISE_SCENARIO_HPP_

#include "scenario.hpp"

class WaterRiseScenario : public Scenario {
	public:
		using	Scenario::Scenario;
		float	getTimeStep() const override;
		void	updateScenario() const override;
};

#endif
