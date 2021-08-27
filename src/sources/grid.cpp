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
