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
#include <Corrade/Containers/ArrayViewStl.h>
#include <Magnum/Shaders/VertexColorGL.h>

using namespace Magnum;

struct Vertex {
	Vector3 position;
	Color4 color;
};

class TerrainDefinition {
	public:
		void	 	debug() const;
		void	 	addPoint(Vector3);
		void 		computeEdges();
		void		normalize();
		float		interpolate(float x, float y, float power) const;
		void		computeMesh();
		GL::Mesh				mesh;
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
