#ifndef _WATER_SIMULATION_HPP_
#define _WATER_SIMULATION_HPP_

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Timeline.h>

#include "terrainDefinition.hpp"
#include "meshCreation.hpp"
#include "grid.hpp"

using namespace Magnum;

class WaterSimulation : public TerrainDefinition, public MeshCreation
{
	protected:
									WaterSimulation();
		void						handleTerrain();
		void						updateSimulation(float timestep);

		std::shared_ptr<Grid>		grid;
		const int					gridSize {static_cast<int> (1.0f / this->precision)};
		float						cellSize {0.1f};
		const float					gravity {9.81f};
		float						viscosity {1.5f};

	private:
		void						updatePipeFlows(float timestep);
		float						updatePipeFlow(const Cell *cell0, const Cell *cell1, float pipeFlow, float timestep);
		void						updateWaterDepth(float timestep);
		float						deltaSurface(const Cell *c0, const Cell *c1) const;
};

#endif
