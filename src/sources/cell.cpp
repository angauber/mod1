#include "cell.hpp"

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
