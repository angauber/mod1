#ifndef _WAVE_SCENARIO_HPP_
#define _WAVE_SCENARIO_HPP_

#include <Magnum/Magnum.h>
#include <Magnum/Math/Functions.h>

#include "scenario.hpp"

using namespace Magnum;

class WaveScenario : public Scenario {
	public:
		using Scenario::Scenario;

		float	getTimeStep() const override;
		void	setupScenario() const override;
};

#endif
