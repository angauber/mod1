#ifndef _RAIN_SCENARIO_HPP_
#define _RAIN_SCENARIO_HPP_

#include "scenario.hpp"

class RainScenario : public Scenario {
	public:
		float	getTimeStep() const override;
		void	setupScenario(SimulationGrid &grid, std::size_t size) const override;
		void	updateScenario(SimulationGrid &grid, std::size_t size) const override;
//		virtual void	drawGUI() override;
};

#endif
