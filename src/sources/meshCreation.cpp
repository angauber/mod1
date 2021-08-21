#include "meshCreation.hpp"

int		id(int i, int j, int size)
{
	return (i * size) + j;
}

GL::Mesh	MeshCreation::createMesh()
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<Vertex> vertices;

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

	/**
	 * Build vertices
	 */
	for (size_t i = 0; i < positions.size(); i++) {
		vertices.push_back(Vertex {this->positions[i], this->normals[i], this->colors[i]});
	}

	/**
	 * Fill buffers
	 */
	verticesBuffer.setData(vertices, GL::BufferUsage::StaticDraw);
	indexBuffer.setData(this->indices);

	/**
	 * Create Mesh
	 */
	mesh.setPrimitive(GL::MeshPrimitive::Triangles)
		.setCount(this->indices.size())
		.addVertexBuffer(std::move(verticesBuffer),
			0,
			Shaders::PhongGL::Position{},
			Shaders::PhongGL::Normal{},
		    Shaders::PhongGL::Color4{}
		).setIndexBuffer(std::move(indexBuffer), 0, GL::MeshIndexType::UnsignedShort, 0, vertices.size() - 1);

	return mesh;
}

void	MeshCreation::fillVectors(int size, const SimulationGrid &grid, std::function<float(Cell)> height, std::function<bool(Cell)> shouldRender, std::function<Color4(Cell)> getColor)
{
	this->positions.clear();
	this->indices.clear();
	this->colors.clear();

	for (int i = 0; i < size; i++) { 
		for (int j = 0; j < size; j++) {
			// use lambda to evaluate height
			this->positions.push_back(Vector3 {static_cast<float> (i) / size, height(grid[i][j]), static_cast<float> (j) / size});
			this->colors.push_back(getColor(grid[i][j]));

			if (shouldRender(grid[i][j])) {
				// Building two triangles, vertex need to be ordered in trigonometric direction
				if (i < size - 1 && j < size - 1) {
					// Upper triangle
					this->indices.push_back(id(i, j, size));
					this->indices.push_back(id(i, j + 1, size));
					this->indices.push_back(id(i + 1, j, size));

					// lower triangle
					this->indices.push_back(id(i + 1, j, size));
					this->indices.push_back(id(i, j + 1, size));
					this->indices.push_back(id(i + 1, j + 1, size));
				}
			}
		}
	}
}

GL::Mesh	MeshCreation::createWaterMesh(int size, const SimulationGrid &grid)
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<Vertex> vertices;

	Color4 start {33.0f / 255, 118.0f / 255, 255.0f / 255, 0.3f};
	Color4 end {33.0f / 255, 92.0f / 255, 255.0f / 255, 1.0f};

	this->fillVectors(
		size,
		grid,
		[] (const Cell &cell) -> float { return cell.terrainHeight + cell.waterDepth; },
		[] (const Cell &cell) -> bool { return cell.isWet(); },
		[start, end] (const Cell &cell) -> Color4 {
			return Math::lerp(start, end, cell.waterDepth);
		}
	);

	/**
	 *  Left side
	 */
	for (int n = 0; n < size; n++) {
		this->positions.push_back(Vector3 {static_cast<float> (n) / size, grid[n][0].terrainHeight, 0.0f});
		this->colors.push_back(start);

		if (n < size - 1 && (grid[n][0].isWet() || grid[n + 1][0].isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(n, 0, size));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(id(n + 1, 0, size));

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(n + 1, 0, size));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());
		}
	}

	/**
	 *  Right side
	 */
	for (int n = 0; n < size; n++) {
		this->positions.push_back(Vector3 {static_cast<float> (n) / size, grid[n][size - 1].terrainHeight, static_cast<float> (size - 1) / size});
		this->colors.push_back(start);

		if (n < size - 1 && (grid[n][size - 1].isWet() || grid[n + 1][size - 1].isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(n, size - 1, size));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(n, size - 1, size));
			this->indices.push_back(this->positions.size());
			this->indices.push_back(id(n + 1, size - 1, size));
		}
	}

	/**
	 *  Back side
	 */
	for (int n = 0; n < size; n++) {
		this->positions.push_back(Vector3 {0.0f, grid[0][n].terrainHeight, static_cast<float> (n) / size});
		this->colors.push_back(start);

		if (n < size - 1 && (grid[0][n].isWet() || grid[0][n + 1].isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(0, n, size));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(id(0, n + 1, size));

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(0, n + 1, size));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());
		}
	}

	/**
	 *  Front side
	 */
	for (int n = 0; n < size; n++) {
		this->positions.push_back(Vector3 {static_cast<float> (size - 1) / size, grid[size - 1][n].terrainHeight, static_cast<float> (n) / size});
		this->colors.push_back(start);

		if (n < size - 1 && (grid[size - 1][n].isWet() || grid[size - 1][n + 1].isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(size - 1, n, size));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(id(size - 1, n + 1, size));

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(size - 1, n + 1, size));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());
		}
	}

	return this->createMesh();
}

GL::Mesh	MeshCreation::createTerrainMesh(int size, const SimulationGrid &grid)
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<Vertex> vertices;

	/**
	 * Could use a multi gradient of some sort
	 */
	this->fillVectors(
			size,
			grid,
			[] (const Cell &cell) -> float { return cell.terrainHeight; },
			[] (const Cell &cell) -> bool { return true; },
			[] (const Cell &cell) -> Color4 {
				Color4 start {143.0f / 255, 79.0f / 255, 31.0f / 255, 1.0};

				return start;
			}
	);

	return this->createMesh();
}

