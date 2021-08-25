#ifndef _MESH_CREATION_HPP_
#define _MESH_CREATION_HPP_

#include <functional>
#include <memory>

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
		void		setGrid(std::shared_ptr<Grid> grid);
		GL::Mesh	createTerrainMesh();
		GL::Mesh	createWaterMesh();
	
		float	terrainStartColor[3] {0.35f, 0.235f, 0.157f};
		float	terrainEndColor[3] {0.35f, 0.235f, 0.157f};

		float	waterStartColor[4] {33.0f / 255, 118.0f / 255, 255.0f / 255, 0.45f};
		float	waterEndColor[4] {33.0f / 255, 92.0f / 255, 255.0f / 255, 1.0f};

	private:
		std::shared_ptr<Grid>		grid;
		std::vector<Vector3>		positions;
		std::vector<UnsignedShort>	indices;
		std::vector<Color4>			colors;
		Containers::Array<Vector3>	normals;

		GL::Mesh	createMesh();
		void		fillVectors(std::function<float(Cell)> height, std::function<bool(Cell)> shouldRender, std::function<Color4(Cell)> getColor);
};

#endif
