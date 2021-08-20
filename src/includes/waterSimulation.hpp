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
#include "scenario.hpp"
#include "waterRiseScenario.hpp"
#include "waterSpreadScenario.hpp"
#include "waveScenario.hpp"

using namespace Magnum;

class WaterSimulation : public TerrainDefinition, public MeshCreation
{
	protected:
						WaterSimulation();
		const int		gridSize {static_cast<int> (1.0f / this->precision)};
		const float		cellSize {0.1f};
		const float		gravity {9.81f};

		void			handleTerrain();
		GL::Mesh		computeTerrainMesh();
		GL::Mesh		computeWaterMesh();
		void			updateSimulation(float timestep);
		SimulationGrid	getSimulationGrid() const;
	private:
		SimulationGrid										grid {this->gridSize, std::vector<Cell> (this->gridSize)};
		std::map<std::string, std::shared_ptr<Scenario>>	scenarios;
		std::string											scenario {"rise"};
		Timeline	timeline;
		float												timeToUpdate {0.0f};

		std::shared_ptr<Scenario>	getScenario();
		void	updatePipeFlows(float timestep);
		float	updatePipeFlow(const Cell &cell0, const Cell &cell1, float pipeFlow, float timestep);
		void	updateWaterDepth(float timestep);
		float	deltaSurface(const Cell &c0, const Cell &c1) const;
};

#endif
