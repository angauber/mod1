#ifndef _WAVE_SCENARIO_HPP_
#define _WAVE_SCENARIO_HPP_

#include "scenario.hpp"

class WaveScenario : public Scenario {
	public:
		using Scenario::Scenario;

		float	getTimeStep() const override;
		void	setupScenario() const override;
};

#endif
