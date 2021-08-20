#include "grid.hpp"

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
