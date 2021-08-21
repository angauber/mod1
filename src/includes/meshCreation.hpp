#ifndef _MESH_CREATION_HPP_
#define _MESH_CREATION_HPP_

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

#include "grid.hpp"

using namespace Magnum;

int		id(int i, int j, int size);

struct Vertex {
	Vector3 position;
	Vector3 normal;
    Color4 color;
};

class MeshCreation
{
	protected:
		GL::Mesh	createTerrainMesh(int size, const SimulationGrid &grid);
		GL::Mesh	createWaterMesh(int size, const SimulationGrid &grid);

	private:
		std::vector<Vector3>		positions;
		std::vector<UnsignedShort>	indices;
		std::vector<Color4>			colors;
		Containers::Array<Vector3>	normals;

		GL::Mesh	createMesh();
		void		fillVectors(int size, const SimulationGrid &grid, std::function<float(Cell)> height, std::function<bool(Cell)> shouldRender, std::function<Color4(Cell)> getColor);
};

#endif
