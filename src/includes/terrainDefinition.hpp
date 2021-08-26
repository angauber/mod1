#ifndef _TERRAIN_DEFINITION_HPP_
#define _TERRAIN_DEFINITION_HPP_

#include <vector>
#include <algorithm>
#include <functional>
#include <Magnum/Math/Distance.h>

#include "mod1Parser.hpp"

using namespace Magnum;

class TerrainDefinition : public Mod1Parser {
	protected:
		const float		precision {0.01f};

		void		handleTerrain();
		void	 	debugTerrain() const;
		float		interpolateTerrain(float x, float y) const;
	private:
		std::vector<Vector3>	points;
		float					minX {0};
		float					maxX {0};
		float					minY {0};
		float					maxY {0};
		float					minZ {0};
		float					maxZ {0};

		void	addPoint(Vector3) override;
		size_t	getPointCount() const override;
		void	computeMinMax();
		void 	computeEdges();
		void	scale();
		std::vector<Vector3>	getClosestPoints(Vector3 point, std::size_t nb) const;
};

#endif
