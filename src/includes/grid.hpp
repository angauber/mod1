#ifndef _GRID_HPP_
#define _GRID_HPP_

#include <vector>

class Cell {
	public:
		float	terrainHeight {0.0f};
		float	waterDepth {0.0f};
		float	downPipe {0.0f};
		float	rightPipe {0.0f};

		bool	isDry() const;
		bool	isWet() const;
		float	surfaceHeight() const;
		void	clear();		
};

typedef std::vector<std::vector<Cell>> SimulationGrid;

#endif
