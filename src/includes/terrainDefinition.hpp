#ifndef _TERRAIN_DEFINITION_HPP_
#define _TERRAIN_DEFINITION_HPP_

#include <vector>
#include <algorithm>
#include <functional>
#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector3.h>
#include <Magnum/Math/Functions.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/MeshTools/GenerateNormals.h>
#include <Corrade/Containers/Array.h>
#include <Corrade/Containers/ArrayView.h>
#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/StridedArrayView.h>

#include "mod1Parser.hpp"

using namespace Magnum;

struct Vertex {
	Vector3 position;
	Vector3 normal;
};

class TerrainDefinition : public Mod1Parser {
	public:
		void	 	debugTerrain() const;
		void	 	addPoint(Vector3) override;
		size_t		getPointCount() const override;
		void 		computeEdges();
		void		scale();
		float		interpolate(float x, float y, float power) const;
		GL::Mesh	computeMesh();
	private:
		std::vector<Vector3>	points;
		float					minX {0};
		float					maxX {0};
		float					minY {0};
		float					maxY {0};
		float					minZ {0};
		float					maxZ {0};

		void	computeMinMax();
};

#endif
