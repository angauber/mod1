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

class Grid {
	public:
				Grid(std::size_t size);
		void	reset();

	private:
		std::vector<std::vector<Cell>>	grid;
		
};

#endif
