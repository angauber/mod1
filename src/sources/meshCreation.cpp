#include "meshCreation.hpp"

void	MeshCreation::fillPositionsIndicesAndNormals(int size, const SimulationGrid &grid, std::function<float(Cell)> height, std::function<bool(Cell)> shouldRender)
{
	this->positions.clear();
	this->indices.clear();

	for (int i = 0; i < size; i++) { 
		for (int j = 0; j < size; j++) {
			// use lambda to evaluate height
			this->positions.push_back(Vector3 {static_cast<float> (i) / size, height(grid[i][j]), static_cast<float> (j) / size});

			if (shouldRender(grid[i][j])) {
				// Building two triangles, vertex need to be ordered in trigonometric direction
				if (i < size - 1 && j < size - 1) {
					// Upper triangle
					this->indices.push_back(positions.size() - 1);
					this->indices.push_back(positions.size());
					this->indices.push_back(positions.size() - 1 + size);
				}

				if (i < size - 1 && j < size - 1) {
					// lower triangle
					this->indices.push_back(positions.size() - 1 + size);
					this->indices.push_back(positions.size());
					this->indices.push_back(positions.size() + size);
				}
			}
		}
	}

	/**
	 * Transform from normalized to -1 +1
	 */
	for (size_t i = 0; i < this->positions.size(); i++) {
		this->positions[i] = (this->positions[i] * Vector3(2.0f)) - Vector3(1.0f);
	}

	/**
	 *  Compute normals for lighting
	 */
	this->normals = MeshTools::generateSmoothNormals(indices, positions);
}

Color3	MeshCreation::getTerrainColor(float height) const
{
	Color3 colors[] = {
//		{70.0f / 255, 50.0f / 255, 20.0f / 255},
		{143.0f / 255, 79.0f / 255, 31.0f / 255},
		{77.0f / 255, 76.0f / 255, 82.0f / 255},
	};

	return Math::lerp(colors[0], colors[1], (height / 2) + 0.5f);
}

Color3	MeshCreation::getWaterColor(float depth) const
{
	Color3 colors[] = {
		{50.0f / 255, 204.0f / 255, 225.0f / 255},
		{33.0f / 255, 92.0f / 255, 255.0f / 255},
	};

	return colors[1];

	// lerping should use the water depth, not it's surface height here
	return Math::lerp(colors[0], colors[1], (depth / 2) + 0.5f);
}

GL::Mesh	MeshCreation::createTerrainMesh(int size, const SimulationGrid &grid)
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<TerrainVertex> vertices;

	this->fillPositionsIndicesAndNormals(
			size,
			grid,
			[] (const Cell &cell) -> float { return cell.terrainHeight; },
			[] (const Cell &cell) -> bool { return true; }
	);


	// Build vertices
	for (size_t i = 0; i < positions.size(); i++) {
		vertices.push_back(TerrainVertex {this->positions[i], this->normals[i], this->getTerrainColor(this->positions[i].y())});
	}

	// Filling buffers
	verticesBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
	indexBuffer.setData(this->indices);

	// Creating mesh
	mesh.setPrimitive(GL::MeshPrimitive::Triangles)
		.setCount(this->indices.size())
		.addVertexBuffer(std::move(verticesBuffer),
			0,
			Shaders::PhongGL::Position{},
			Shaders::PhongGL::Normal{},
		    Shaders::PhongGL::Color3{}
		).setIndexBuffer(std::move(indexBuffer), 0, GL::MeshIndexType::UnsignedShort, 0, vertices.size() - 1);

	return mesh;
}

GL::Mesh	MeshCreation::createWaterMesh(int size, const SimulationGrid &grid)
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<WaterVertex> vertices;

	this->fillPositionsIndicesAndNormals(
			size,
			grid,
			[] (const Cell &cell) -> float { return cell.terrainHeight + cell.waterDepth; },
			[] (const Cell &cell) -> bool { return cell.waterDepth > 0.001f; }
	);


	// Build vertices
	for (size_t i = 0; i < this->positions.size(); i++) {
		vertices.push_back(WaterVertex {positions[i], normals[i], this->getWaterColor(this->positions[i].y())});
	}

	// Filling buffers
	verticesBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
	indexBuffer.setData(this->indices);

	// Creating mesh
	mesh.setPrimitive(GL::MeshPrimitive::Triangles)
		.setCount(this->indices.size())
		.addVertexBuffer(std::move(verticesBuffer),
			0,
			Shaders::PhongGL::Position{},
			Shaders::PhongGL::Normal{},
			Shaders::PhongGL::Color3{}
		).setIndexBuffer(std::move(indexBuffer), 0, GL::MeshIndexType::UnsignedShort, 0, vertices.size() - 1);

	return mesh;
}
