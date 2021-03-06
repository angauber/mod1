#include "waterSimulation.hpp"

WaterSimulation::WaterSimulation() 
{
	this->grid = std::make_shared<Grid> (this->gridSize);

	this->setGrid(this->grid);
}

void	WaterSimulation::handleTerrain()
{
	TerrainDefinition::handleTerrain();

	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			this->grid->get(i, j)->terrainHeight = this->interpolateTerrain(i * this->precision, j * this->precision);
		}
	}
}

void	WaterSimulation::updateSimulation(float timestep)
{
	this->updatePipeFlows(timestep);
	this->updateWaterDepth(timestep);
}

void	WaterSimulation::updatePipeFlows(float timestep)
{
	Cell *cell;

	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			cell = this->grid->get(i, j);

			if (j < this->gridSize - 1)
				cell->rightPipe = this->updatePipeFlow(cell, this->grid->get(i, j + 1), cell->rightPipe, timestep);
		
			if (i < this->gridSize - 1)
				cell->downPipe = this->updatePipeFlow(cell, this->grid->get(i + 1, j), cell->downPipe, timestep);
		}
	}
}

float	WaterSimulation::updatePipeFlow(const Cell *cell0, const Cell *cell1, float pipeFlow, float timestep)
{
	float pipeCrossSection {this->cellSize};
	float deltaSurface {0.0f};

	/**
	 *  If one of the two cells is dry and the other has a water height inferior to the terrainHeight of the first one
	 *  block the flow
	 */
	if ((cell1->isDry() && cell0->surfaceHeight() < cell1->terrainHeight) ||
		(cell0->isDry() && cell1->surfaceHeight() < cell0->terrainHeight)) {
		return 0.0f;
	}

	if (cell0->surfaceHeight() > cell1->surfaceHeight()) {
		deltaSurface = this->deltaSurface(cell0, cell1);
		deltaSurface = deltaSurface > cell0->waterDepth ? cell0->waterDepth : deltaSurface;
		deltaSurface *= -1;
	} else {
		deltaSurface = this->deltaSurface(cell1, cell0);
		deltaSurface = deltaSurface > cell1->waterDepth ? cell1->waterDepth : deltaSurface;
	}

	pipeCrossSection *= std::abs(deltaSurface) * this->viscosity;

	pipeFlow += pipeCrossSection * (this->gravity / this->cellSize) * deltaSurface * timestep;

	/**
	 * Improve stability
	 */
	return pipeFlow * std::clamp(1.0f - (timestep / 3.0f), 0.0f, 1.0f);
}

void	WaterSimulation::updateWaterDepth(float timestep)
{
	float pipeFlowSum {0.0f};
	Cell *cell;

	for (int i = 0; i < this->gridSize; i++) {
		for (int j = 0; j < this->gridSize; j++) {
			cell = this->grid->get(i, j);

			// Neumann neighbouring based sum
			pipeFlowSum = 0.0f;

			// Left Pipe
			if (j > 0)
				pipeFlowSum += this->grid->get(i, j - 1)->rightPipe;
			// Top Pipe
			if (i > 0)
				pipeFlowSum += this->grid->get(i - 1, j)->downPipe;
			// Right Pipe
			if (j < this->gridSize - 1)
				pipeFlowSum -= cell->rightPipe;
			// Bottom Pipe
			if (i < this->gridSize - 1)
				pipeFlowSum -= cell->downPipe;

			cell->waterDepth += -1.0f * timestep * (pipeFlowSum / (this->cellSize * this->cellSize));

			// hacky
			cell->waterDepth = std::clamp(cell->waterDepth, 0.0f, 2.0f);
		}
	}

}

float	WaterSimulation::deltaSurface(const Cell *c0, const Cell *c1) const
{
	return c0->surfaceHeight() - c1->surfaceHeight();
}
