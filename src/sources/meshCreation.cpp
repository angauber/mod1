#include "meshCreation.hpp"

void		MeshCreation::setGrid(std::shared_ptr<Grid> grid)
{
	this->grid = grid;
}

std::size_t		MeshCreation::id(std::size_t i, std::size_t j)
{
	return (i * this->grid->size) + j;
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
	for (std::size_t i = 0; i < this->positions.size(); i++) {
		this->positions[i] = (this->positions[i] * Vector3(2.0f)) - Vector3(1.0f);
	}


	/**
	 *  Compute normals for lighting
	 */
	this->normals = MeshTools::generateSmoothNormals(this->indices, this->positions);

	/**
	 * Build vertices
	 */
	for (std::size_t i = 0; i < this->positions.size(); i++) {
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

void	MeshCreation::fillVectors(std::function<float(Cell *)> height, std::function<bool(Cell *)> shouldRender, std::function<Color4(Cell *)> getColor)
{
	this->positions.clear();
	this->indices.clear();
	this->colors.clear();

	for (std::size_t i = 0; i < this->grid->size; i++) { 
		for (std::size_t j = 0; j < this->grid->size; j++) {
			// use lambda to evaluate height
			this->positions.push_back(Vector3 {static_cast<float> (i) / this->grid->size, height(this->grid->get(i, j)), static_cast<float> (j) / this->grid->size});
			this->colors.push_back(getColor(this->grid->get(i, j)));

				// Building two triangles, vertex need to be ordered in trigonometric direction
			if (i < this->grid->size - 1 && j < this->grid->size - 1) {
				if (shouldRender(this->grid->get(i, j)) && shouldRender(this->grid->get(i, j + 1)) && shouldRender(this->grid->get(i + 1, j))) {
					// Upper triangle
					this->indices.push_back(this->id(i, j));
					this->indices.push_back(this->id(i, j + 1));
					this->indices.push_back(this->id(i + 1, j));
				}

				if (shouldRender(this->grid->get(i + 1, j)) && shouldRender(this->grid->get(i, j + 1)) && shouldRender(this->grid->get(i + 1, j + 1))) {
					// lower triangle
					this->indices.push_back(id(i + 1, j));
					this->indices.push_back(id(i, j + 1));
					this->indices.push_back(id(i + 1, j + 1));
				}
			}
		}
	}
}

GL::Mesh	MeshCreation::createTerrainMesh()
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<Vertex> vertices;

	Color4 start {this->terrainStartColor[0], this->terrainStartColor[1], this->terrainStartColor[2], 1.0f};
	Color4 end {this->terrainEndColor[0], this->terrainEndColor[1], this->terrainEndColor[2], 1.0f};

	/**
	 * Could use a multi gradient of some sort
	 */
	this->fillVectors(
			[] (const Cell *cell) -> float { return cell->terrainHeight; },
			[] (const Cell *cell) -> bool { return true; },
			[start, end] (const Cell *cell) -> Color4 {
				return Math::lerp(start, end, cell->terrainHeight * 4.0f);
			}
	);

	return this->createMesh();
}

GL::Mesh	MeshCreation::createWaterMesh()
{
	GL::Mesh mesh;
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	std::vector<Vertex> vertices;

	Color4 start {this->waterStartColor[0], this->waterStartColor[1], this->waterStartColor[2], this->waterStartColor[3]};
	Color4 end {this->waterEndColor[0], this->waterEndColor[1], this->waterEndColor[2], this->waterEndColor[3]};

	this->fillVectors(
		[] (const Cell *cell) -> float { return cell->surfaceHeight(); },
		[] (const Cell *cell) -> bool { return cell->isWet(); },
		[start, end] (const Cell *cell) -> Color4 {
			return Math::lerp(start, end, cell->waterDepth);
		}
	);

	/**
	 *  Left side
	 */
	for (std::size_t n = 0; n < this->grid->size; n++) {
		this->positions.push_back(Vector3 {static_cast<float> (n) / this->grid->size, this->grid->get(n, 0)->terrainHeight, 0.0f});
		this->colors.push_back(Math::lerp(start, end, this->grid->get(n, 0)->waterDepth));

		if (n < this->grid->size - 1 && (this->grid->get(n, 0)->isWet() || this->grid->get(n + 1, 0)->isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(n, 0));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(id(n + 1, 0));

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(n + 1, 0));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());
		}
	}

	/**
	 *  Right side
	 */
	for (std::size_t n = 0; n < this->grid->size; n++) {
		this->positions.push_back(Vector3 {static_cast<float> (n) / this->grid->size, this->grid->get(n, this->grid->size - 1)->terrainHeight, static_cast<float> (this->grid->size - 1) / this->grid->size});
		this->colors.push_back(Math::lerp(start, end, this->grid->get(n, this->grid->size - 1)->waterDepth));

		if (n < this->grid->size - 1 && (this->grid->get(n, this->grid->size - 1)->isWet() || this->grid->get(n + 1, this->grid->size - 1)->isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(n, this->grid->size - 1));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(n, this->grid->size - 1));
			this->indices.push_back(this->positions.size());
			this->indices.push_back(id(n + 1, this->grid->size - 1));
		}
	}

	/**
	 *  Back side
	 */
	for (std::size_t n = 0; n < this->grid->size; n++) {
		this->positions.push_back(Vector3 {0.0f, this->grid->get(0, n)->terrainHeight, static_cast<float> (n) / this->grid->size});
		this->colors.push_back(Math::lerp(start, end, this->grid->get(0, n)->waterDepth));

		if (n < this->grid->size - 1 && (this->grid->get(0, n)->isWet() || this->grid->get(0, n + 1)->isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(0, n));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(id(0, n + 1));

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(0, n + 1));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());
		}
	}

	/**
	 *  Front side
	 */
	for (std::size_t n = 0; n < this->grid->size; n++) {
		this->positions.push_back(Vector3 {static_cast<float> (this->grid->size - 1) / this->grid->size, this->grid->get(this->grid->size - 1, n)->terrainHeight, static_cast<float> (n) / this->grid->size});
		this->colors.push_back(Math::lerp(start, end, this->grid->get(this->grid->size - 1, n)->waterDepth));

		if (n < this->grid->size - 1 && (this->grid->get(this->grid->size - 1, n)->isWet() || this->grid->get(this->grid->size - 1, n + 1)->isWet())) {
			/**
			 * Upper triangle
			 */
			this->indices.push_back(id(this->grid->size - 1, n));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(id(this->grid->size - 1, n + 1));

			/**
			 * Lower triangle
			 */
			this->indices.push_back(id(this->grid->size - 1, n + 1));
			this->indices.push_back(this->positions.size() - 1);
			this->indices.push_back(this->positions.size());
		}
	}

	return this->createMesh();
}
