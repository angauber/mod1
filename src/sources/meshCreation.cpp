#include "meshCreation.hpp"

GL::Mesh	MeshCreation::createMesh(int size, SimulationGrid grid, std::function<float(Cell)> height, std::function<bool(Cell)> shouldRender)
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<Vector3> positions;
	std::vector<UnsignedShort> indices;
	std::vector<Vertex> vertices;

	for (int i = 0; i < size; i++) { 
		for (int j = 0; j < size; j++) {
			// use lambda to evaluate height
			positions.push_back(Vector3 {static_cast<float> (i) / size, height(grid[i][j]), static_cast<float> (j) / size});

			if (shouldRender(grid[i][j])) {
				// Building two triangles, vertex need to be ordered in trigonometric direction
				if (i < size - 1 && j < size - 1) {
					// Upper triangle
					indices.push_back(positions.size() - 1);
					indices.push_back(positions.size());
					indices.push_back(positions.size() - 1 + size);
				}

				if (i < size - 1 && j < size - 1) {
					// lower triangle
					indices.push_back(positions.size() - 1 + size);
					indices.push_back(positions.size());
					indices.push_back(positions.size() + size);
				}
			}
		}
	}

	// Transform from normalized to -1 +1
	for (size_t i = 0; i < positions.size(); i++) {
		positions[i] = (positions[i] * Vector3(2.0f)) - Vector3(1.0f);
	}

	// Compute normals for lighting
	Containers::Array<Vector3> normals = MeshTools::generateSmoothNormals(indices, positions);

	// Build vertices
	for (size_t i = 0; i < positions.size(); i++) {
		vertices.push_back(Vertex {positions[i], normals[i]});
	}

	// Filling buffers
	verticesBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
	indexBuffer.setData(indices);

	/*
	Debug{} << "Vertices:" << vertices.size();
	Debug{} << "Triangles:" << indices.size() / 3;
	*/

	// Creating mesh
	mesh.setPrimitive(GL::MeshPrimitive::Triangles)
		.setCount(indices.size())
		.addVertexBuffer(std::move(verticesBuffer), // should be owned elsewhere for water
			0,
			Shaders::PhongGL::Position{},
			Shaders::PhongGL::Normal{}
		).setIndexBuffer(std::move(indexBuffer), 0, GL::MeshIndexType::UnsignedShort, 0, vertices.size() - 1);

	return mesh;
}
