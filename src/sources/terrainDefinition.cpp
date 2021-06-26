#include "terrainDefinition.hpp"

void	TerrainDefinition::debugTerrain() const
{
	for (const auto &point : this->points) {
		Debug{} << point;
	}
}

void	TerrainDefinition::addPoint(Vector3 vector)
{
	this->points.push_back(vector);
}

size_t	TerrainDefinition::getPointCount() const
{
	return this->points.size();
}

void	TerrainDefinition::computeEdges()
{
	int padding = 5000;

	this->computeMinMax();

	this->minX -= padding;
	this->maxX += padding;
	this->minY -= padding;
	this->maxY += padding;

	this->addPoint(Vector3 {this->minX, this->minY, 0});
	this->addPoint(Vector3 {this->minX, this->maxY, 0});
	this->addPoint(Vector3 {this->maxX, this->minY, 0});
	this->addPoint(Vector3 {this->maxX, this->maxY, 0});

	this->computeMinMax();
}

void	TerrainDefinition::computeMinMax()
{
	std::function<bool (const Vector3 &, const Vector3 &)> xCompare = [] (const Vector3 &lhs, const Vector3 &rhs) {
		return lhs.x() < rhs.x();
	};

	std::function<bool (const Vector3 &, const Vector3 &)> yCompare = [] (const Vector3 &lhs, const Vector3 &rhs) {
		return lhs.y() < rhs.y();
	};

	std::function<bool (const Vector3 &, const Vector3 &)> zCompare = [] (const Vector3 &lhs, const Vector3 &rhs) {
		return lhs.z() < rhs.z();
	};

	this->minX = std::min_element(this->points.begin(), this->points.end(), xCompare)->x();
	this->maxX = std::max_element(this->points.begin(), this->points.end(), xCompare)->x();
	this->minY = std::min_element(this->points.begin(), this->points.end(), yCompare)->y();
	this->maxY = std::max_element(this->points.begin(), this->points.end(), yCompare)->y();
	this->minZ = std::min_element(this->points.begin(), this->points.end(), zCompare)->z();
	this->maxZ = std::max_element(this->points.begin(), this->points.end(), zCompare)->z();
}

/**
 * Scale all the points down to fit in a (0; 1) box
 * Might need fixing on negative coords (x and y)
 */
void	TerrainDefinition::scale()
{
	for (auto &point : this->points) {
		point -= Vector3{this->minX, this->minY, 0};
	}

	this->computeMinMax();

	float norm = std::max(this->maxX, this->maxY);

	Vector3 size {};

	for (auto &point : this->points) {
		point /= norm;
	}

	this->computeMinMax();
}

/**
 *  use IDW to interpolate coordinates
 */
float	TerrainDefinition::interpolate(float x, float y, const float power) const
{
	float a {0.0f};
	float b {0.0f};
	float distance;
	float weight;

	for (const auto &point : this->points) {
		if (x == point.x() && y == point.y()) {
			return point.z();
		}

		distance = sqrt(pow(x - point.x(), 2.0f) + pow(y - point.y(), 2.0f));
		weight = pow(1.0f / distance, power);

		a += weight * point.z();
		b += weight;
	}

	return b == 0.0f ? 0.0f : a / b;
}

GL::Mesh	TerrainDefinition::computeMesh()
{
	GL::Mesh mesh;	
	GL::Buffer verticesBuffer;
	GL::Buffer indexBuffer;
	float power {2.0};
	float precision {0.01f};
	float x {0.0f};
	float y {0.0f};
	float z {0.0f};
	int cols {static_cast<int> ((this->maxX - this->minX) / precision)};
	int rows {static_cast<int> ((this->maxY - this->minY) / precision)};
	std::vector<Vector3> positions;
	std::vector<UnsignedShort> indices;
	std::vector<Vertex> vertices;

	for (int i = 0; i < cols; i++) { 
		for (int j = 0; j < rows; j++) {
			x = this->minX + i * precision;
			y = this->minY + j * precision;
			z = this->interpolate(x, y, power);

			positions.push_back(Vector3 {x, static_cast<Float> (z), y});

			// Building two triangles, vertex need to be ordered in trigonometric direction
			if (i < cols - 1 && j < rows - 1) {
				// Upper triangle
				indices.push_back(positions.size() - 1);
				indices.push_back(positions.size());
				indices.push_back(positions.size() - 1 + rows);
			}

			if (i < cols - 2 && j < rows - 2) {
				// lower triangle
				indices.push_back(positions.size() - 1 + rows);
				indices.push_back(positions.size());
				indices.push_back(positions.size() + rows);
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

	Debug{} << "Vertices:" << vertices.size();
	Debug{} << "Triangles:" << indices.size() / 3;

	// Creating mesh
	mesh.setPrimitive(GL::MeshPrimitive::Triangles)
		.setCount(indices.size())
		.addVertexBuffer(verticesBuffer,
			0,
			Shaders::PhongGL::Position{},
			Shaders::PhongGL::Normal{}
		).setIndexBuffer(indexBuffer, 0, GL::MeshIndexType::UnsignedShort, 0, vertices.size() - 1);

	return mesh;
}
