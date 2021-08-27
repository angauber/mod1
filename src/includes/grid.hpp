#ifndef _GRID_HPP_
#define _GRID_HPP_

#include <vector>

#include "cell.hpp"

class Grid {
	public:
		const std::size_t	size;

							Grid(std::size_t size);
		
		void				reset();
		Cell *				get(std::size_t i, std::size_t j);

	private:
		std::vector<Cell>	grid;
		
};

#endif
