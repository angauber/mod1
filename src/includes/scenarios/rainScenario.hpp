#ifndef _RAIN_SCENARIO_HPP_
#define _RAIN_SCENARIO_HPP_

#include "scenario.hpp"

class RainScenario : public Scenario {
	public:
		using	Scenario::Scenario;
		float	getTimeStep() const override;
		void	updateScenario() const override;
		virtual void	drawGUI() override;
	private:
		float	rise {10.0f};
};

#endif
