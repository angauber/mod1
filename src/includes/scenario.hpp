#ifndef _SCENARIO_HPP_
#define _SCENARIO_HPP_

#include "grid.hpp"

class Scenario {
	public:	
		virtual float	getTimeStep() const = 0;
		virtual	void	setupScenario(SimulationGrid &grid, std::size_t size) const = 0;
		virtual void	updateScenario(SimulationGrid &grid, std::size_t size) const = 0;
};

#endif
