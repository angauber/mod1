#ifndef _WATER_SPREAD_SCENARIO_HPP_
#define _WATER_SPREAD_SCENARIO_HPP_

#include "scenario.hpp"

class WaterSpreadScenario : public Scenario {
	public:
		using	Scenario::Scenario;
		float	getTimeStep() const override;
		void	setupScenario() const override;
};

#endif
