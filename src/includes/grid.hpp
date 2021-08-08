#ifndef _GRID_HPP_
#define _GRID_HPP_

#include <vector>

struct Cell {
	float	terrainHeight {0.0f};
	float	waterDepth {0.0f};
	float	downPipe {0.0f};
	float	rightPipe {0.0f};
};

typedef std::vector<std::vector<Cell>> SimulationGrid;

#endif
