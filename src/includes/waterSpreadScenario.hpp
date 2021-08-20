#ifndef _WATER_SPREAD_SCENARIO_HPP_
#define _WATER_SPREAD_SCENARIO_HPP_

#include "scenario.hpp"

class WaterSpreadScenario : public Scenario {
	public:
		float	getTimeStep() const override;
		void	setupScenario(SimulationGrid &grid, std::size_t size) const override;
		void	updateScenario(SimulationGrid &grid, std::size_t size) const override;
};

#endif
