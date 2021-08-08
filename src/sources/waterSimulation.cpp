#include "waterSimulation.hpp"

SimulationGrid	WaterSimulation::getSimulationGrid() const
{
	return this->grid;
}

void	WaterSimulation::handleTerrain()
{
	TerrainDefinition::handleTerrain();

	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			this->grid[i][j].terrainHeight = this->interpolateTerrain(i * this->precision, j * this->precision);
			if (i > 40 && i < 60 && j > 40 && j < 60)
				this->grid[i][j].waterDepth = 0.4f;
		}
	}
}

GL::Mesh	WaterSimulation::computeTerrainMesh()
{
	return this->createMesh(this->gridSize,
							this->grid,
							[] (Cell cell) -> float { return cell.terrainHeight; },
							[] (Cell cell) -> bool { return true; }
	);
}

GL::Mesh	WaterSimulation::computeWaterMesh()
{
	return this->createMesh(this->gridSize,
							this->grid,
							[] (Cell cell) -> float { return cell.terrainHeight + cell.waterDepth; },
							[] (Cell cell) -> bool { return cell.waterDepth > 0.00001f; }
	);
}

void	WaterSimulation::updateSimulation(float timestep)
{
	this->updatePipeFlows(timestep);
	this->updateWaterDepth(timestep);
}

void	WaterSimulation::updatePipeFlows(float timestep)
{
	// Pipe cross section
	float a {0.1f};
	float spacing {0.5f};

	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			// right pipe
			if (j < this->gridSize - 1)
				this->grid[i][j].rightPipe += a * (9.81f / spacing) * this->deltaSurface(this->grid[i][j], this->grid[i][j + 1]) * timestep;
			// down pipe
			if (i < this->gridSize - 1)
				this->grid[i][j].downPipe += a * (9.81f / spacing) * this->deltaSurface(this->grid[i][j], this->grid[i + 1][j]) * timestep;
		}
	}
}

void	WaterSimulation::updateWaterDepth(float timestep)
{
	float spacing {0.5f};
	float pipeFlowSum {0.0f};

	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			// Neumann neighbouring based sum
			pipeFlowSum = 0.0f;

			// Left Pipe
			if (j > 0)
				pipeFlowSum += this->grid[i][j - 1].rightPipe;
			// Top Pipe
			if (i > 0)
				pipeFlowSum += this->grid[i - 1][j].downPipe;
			// Right Pipe
			pipeFlowSum -= this->grid[i][j].rightPipe;
			// Bottom Pipe
			pipeFlowSum -= this->grid[i][j].downPipe;

			this->grid[i][j].waterDepth += -1.0f * timestep * (pipeFlowSum / (spacing * spacing));
			// hack
			this->grid[i][j].waterDepth = std::clamp(this->grid[i][j].waterDepth, 0.0f, 1.0f);
		}
	}

}

float	WaterSimulation::deltaSurface(Cell c0, Cell c1) const
{
	return ((c1.terrainHeight + c1.waterDepth) - (c0.terrainHeight + c0.waterDepth));
}
