#ifndef _WATER_RISE_SCENARIO_HPP_
#define _WATER_RISE_SCENARIO_HPP_

#include "scenario.hpp"

class WaterRiseScenario : public Scenario {
	public:
		float	getTimeStep() const override;
		void	setupScenario(SimulationGrid &grid, std::size_t size) const override;
		void	updateScenario(SimulationGrid &grid, std::size_t size) const override;
};

#endif
