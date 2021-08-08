#ifndef _WATER_SIMULATION_HPP_
#define _WATER_SIMULATION_HPP_

#include <vector>
#include <Magnum/GL/Mesh.h>

#include "terrainDefinition.hpp"
#include "meshCreation.hpp"
#include "grid.hpp"

using namespace Magnum;

class WaterSimulation : public TerrainDefinition, public MeshCreation
{
	protected:
		int		gridSize {static_cast<int> (1.0f / this->precision)};

		void			handleTerrain();
		GL::Mesh		computeTerrainMesh();
		GL::Mesh		computeWaterMesh();
		void			updateSimulation(float timestep);
		SimulationGrid	getSimulationGrid() const;
	private:
		SimulationGrid	grid {this->gridSize, std::vector<Cell> (this->gridSize)};

		void	updatePipeFlows(float timestep);
		void	updateWaterDepth(float timestep);
		float	deltaSurface(Cell c0, Cell c1) const;
};

#endif
