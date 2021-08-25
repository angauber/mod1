#include "grid.hpp"

Grid::Grid(std::size_t size) : size {size}, grid {size * size} {};

void	Grid::reset()
{
	for (std::size_t i = 0; i < this->grid.size(); i++) {
		this->grid[i].clear();
	}
}

Cell *	Grid::get(std::size_t i, std::size_t j)
{
	return &this->grid[i * this->size + j];
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
