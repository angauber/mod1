#include "grid.hpp"

Grid::Grid(std::size_t size) : this->grid {size, std::vector<Cell> size}

void	Grid::reset()
{
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
			grid[i][j].clear();
		}
	}
}

bool	Cell::isDry() const
{
	return !this->isWet();
}

bool	Cell::isWet() const
{
	return this->waterDepth > 0.001f;
}

float	Cell::surfaceHeight() const
{
	return this->terrainHeight + this->waterDepth;
}

void	Cell::clear()
{
	this->waterDepth = 0.0f;
	this->rightPipe = 0.0f;
	this->downPipe = 0.0f;
}
