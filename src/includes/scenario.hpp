#ifndef _SCENARIO_HPP_
#define _SCENARIO_HPP_

#include <memory>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include "grid.hpp"

class Scenario {
	public:
						Scenario(std::shared_ptr<Grid> grid);
		virtual float	getTimeStep() const = 0;
		virtual	void	setupScenario() const;
		virtual void	updateScenario() const;
		virtual void	drawGUI();
	protected:
		std::shared_ptr<Grid>	grid;
};

#endif
