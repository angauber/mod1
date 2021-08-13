#include "waterSimulation.hpp"

SimulationGrid	WaterSimulation::getSimulationGrid() const
{
	return this->grid;
}

void	WaterSimulation::handleTerrain()
{
	TerrainDefinition::handleTerrain();

	// setup simulation scenario
	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			this->grid[i][j].terrainHeight = this->interpolateTerrain(i * this->precision, j * this->precision);
			if (i > 40 && i < 60 && j > 40 && j < 60)
				this->grid[i][j].waterDepth = 0.2f;
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
	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			if (j < this->gridSize - 1) {
				this->grid[i][j].rightPipe = this->updatePipeFlow(this->grid[i][j], this->grid[i][j + 1], this->grid[i][j].rightPipe, timestep);
			}
			if (i < this->gridSize - 1) {
				this->grid[i][j].downPipe = this->updatePipeFlow(this->grid[i][j], this->grid[i + 1][j], this->grid[i][j].downPipe, timestep);
			}
		}
	}
}

float	WaterSimulation::updatePipeFlow(Cell &cell0, Cell &cell1, float pipeFlow, float timestep)
{
	float pipeCrossSection {this->cellSize};
	float deltaSurface {0.0f};

	/**
	 *  If one of the two cells is dry and the other has a water height inferior to the terrainHeight of the first one, block the flow
	 */
	if ((this->isDry(cell1) && this->waterHeight(cell0) < cell1.terrainHeight) ||
		(this->isDry(cell0) && this->waterHeight(cell1) < cell0.terrainHeight)) {
		return 0.0f;
	}

	if (this->waterHeight(cell0) > this->waterHeight(cell1)) {
		deltaSurface = this->deltaSurface(cell0, cell1);
		deltaSurface = deltaSurface > cell0.waterDepth ? cell0.waterDepth : deltaSurface;
		deltaSurface *= -1;
	} else {
		deltaSurface = this->deltaSurface(cell1, cell0);
		deltaSurface = deltaSurface > cell1.waterDepth ? cell1.waterDepth : deltaSurface;
	}

	pipeCrossSection *= std::abs(deltaSurface);

	pipeFlow += pipeCrossSection * (this->gravity / this->cellSize) * deltaSurface * timestep;

	return pipeFlow *= (1.0f - (timestep / 3));
}

void	WaterSimulation::updateWaterDepth(float timestep)
{
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
			if (j < this->gridSize - 1)
				pipeFlowSum -= this->grid[i][j].rightPipe;

			// Bottom Pipe
			if (i < this->gridSize - 1)
				pipeFlowSum -= this->grid[i][j].downPipe;

			this->grid[i][j].waterDepth += -1.0f * timestep * (pipeFlowSum / (this->cellSize * this->cellSize));

			// hacky
			this->grid[i][j].waterDepth = std::clamp(this->grid[i][j].waterDepth, 0.0f, 1.0f);
		}
	}

}

bool	WaterSimulation::isDry(Cell cell) const
{
	return cell.waterDepth == 0.0f;
}

float	WaterSimulation::waterHeight(Cell cell) const
{
	return cell.terrainHeight + cell.waterDepth;
}

float	WaterSimulation::deltaSurface(Cell c0, Cell c1) const
{
	return this->waterHeight(c0) - this->waterHeight(c1);
}
