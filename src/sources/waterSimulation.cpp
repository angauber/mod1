#include "waterSimulation.hpp"

WaterSimulation::WaterSimulation()
{
	this->scenarios["rise"] = std::make_shared<WaterRiseScenario> ();
	this->scenarios["spread"] = std::make_shared<WaterSpreadScenario> ();
	this->scenarios["wave"] = std::make_shared<WaveScenario> ();
	this->scenarios["rain"] = std::make_shared<RainScenario> ();

	this->scenario = "rise";

	this->timeline.start();
}

std::shared_ptr<Scenario>	WaterSimulation::getScenario()
{
	if (this->scenarios.contains(this->scenario)) {
		return this->scenarios[this->scenario];
	}

	throw std::runtime_error(std::string {"Scenario: "} + this->scenario + std::string {"does not exists"});
}

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
		}
	}

	this->getScenario()->setupScenario(this->grid, this->gridSize);
}

GL::Mesh	WaterSimulation::computeTerrainMesh()
{
	return this->createTerrainMesh(this->gridSize, this->grid);
}

GL::Mesh	WaterSimulation::computeWaterMesh()
{
	return this->createWaterMesh(this->gridSize, this->grid);
}

void	WaterSimulation::updateSimulation(float timestep)
{
	auto scenario = this->getScenario();

	if (scenario->getTimeStep() > 0.0f) {
		if (this->timeToUpdate > scenario->getTimeStep()) {
			scenario->updateScenario(this->grid, this->gridSize);
			this->timeToUpdate = 0.0f;
		}
		this->timeToUpdate += this->timeline.previousFrameDuration();
	}

	this->updatePipeFlows(timestep);
	this->updateWaterDepth(timestep);

	this->timeline.nextFrame();
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

float	WaterSimulation::updatePipeFlow(const Cell &cell0, const Cell &cell1, float pipeFlow, float timestep)
{
	float pipeCrossSection {this->cellSize};
	float deltaSurface {0.0f};

	/**
	 *  If one of the two cells is dry and the other has a water height inferior to the terrainHeight of the first one
	 *  block the flow
	 */
	if ((cell1.isDry() && cell0.surfaceHeight() < cell1.terrainHeight) ||
		(cell0.isDry() && cell1.surfaceHeight() < cell0.terrainHeight)) {
		return 0.0f;
	}

	if (cell0.surfaceHeight() > cell1.surfaceHeight()) {
		deltaSurface = this->deltaSurface(cell0, cell1);
		deltaSurface = deltaSurface > cell0.waterDepth ? cell0.waterDepth : deltaSurface;
		deltaSurface *= -1;
	} else {
		deltaSurface = this->deltaSurface(cell1, cell0);
		deltaSurface = deltaSurface > cell1.waterDepth ? cell1.waterDepth : deltaSurface;
	}

	pipeCrossSection *= std::abs(deltaSurface) * 1.5f;

	pipeFlow += pipeCrossSection * (this->gravity / this->cellSize) * deltaSurface * timestep;

	/**
	 * Improve stability
	 */
	return pipeFlow * std::clamp(1.0f - (timestep / 3.0f), 0.0f, 1.0f);
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

float	WaterSimulation::deltaSurface(const Cell &c0, const Cell &c1) const
{
	return c0.surfaceHeight() - c1.surfaceHeight();
}
