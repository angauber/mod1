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

struct Vertex {
	Vector3 position;
	Vector3 normal;
//	Color3 color;
};

class MeshCreation
{
	protected:
		GL::Mesh	createMesh(int size, SimulationGrid grid, std::function<float (Cell)> height, std::function<bool(Cell)> shouldRender);
};

#endif