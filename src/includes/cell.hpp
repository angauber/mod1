#ifndef _CELL_HPP_
#define _CELL_HPP_

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

#endif
