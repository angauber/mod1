#include "landDefinition.hpp"

void	LandDefinition::debug() const
{
	for (const auto &point : this->points) {
		Debug{} << point;
	}
}

void	LandDefinition::addPoint(Vector3 vector)
{
	this->points.push_back(vector);
}

void	LandDefinition::computeEdges()
{
	int padding = 2000;

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

void	LandDefinition::computeMinMax()
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

void	LandDefinition::normalize()
{
	float len {1.0f / this->points.size()};
	Vector3 min {this->minX, this->minY, this->minZ};
	Vector3 max {this->maxX, this->maxY, this->maxZ};
	Vector3 delta {max - min};

	/*
	// Bring back x and y to their origin (0; 0)
	for (auto &point : this->points) {
		point -= Vector3{this->minX, this->minY, 0};
	}
	*/

	/**
	 *  Avoiding division by zero
	 */
	if (delta.x() == 0) {
		delta += Vector3::xAxis(len);
	}
	if (delta.y() == 0) {
		delta += Vector3::yAxis(len);
	}
	if (delta.z() == 0) {
		delta += Vector3::zAxis(len);
	}

	/**
	 * Normalizing
	 */
	for (auto &point : this->points) {
		point = (point - min) / delta;
	}

	this->computeMinMax();
}

/**
 *  use IDW to interpolate coordinates
 */
float	LandDefinition::interpolate(float x, float y, const float power) const
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

void	LandDefinition::computeMesh()
{
	GL::Buffer buffer;
	float power {2.0};
	float precision {0.01f};
	float z0 {0.0f};
	float z1 {0.0f};
	float z2 {0.0f};
	float z3 {0.0f};
	Color4 color (66.0f / 255, 135.0f / 255, 245.0f / 255, 1.0f);
	std::vector<Vertex> vertices;

	for (float x = this->minX; x < this->maxX - precision; x += precision) { 
		for (float y = this->minY; y < this->maxY - precision; y += precision) {
			// Building two triangles, vertex need to be ordered in trigonometric direction
			
			z0 = this->interpolate(x, y, power);
			z1 = this->interpolate(x + precision, y, power);
			z2 = this->interpolate(x, y + precision, power);
			z3 = this->interpolate(x + precision, y + precision, power);

			// Upper triangle
			vertices.push_back(Vertex {{x, static_cast<Float> (z0), y}, color});
			vertices.push_back(Vertex {{x, static_cast<Float> (z2), y + precision}, color});
			vertices.push_back(Vertex {{x + precision, static_cast<Float> (z1), y}, color});

			// lower triangle
			vertices.push_back(Vertex {{x + precision, static_cast<Float> (z1), y}, color});
			vertices.push_back(Vertex {{x, static_cast<Float> (z2), y + precision}, color});
			vertices.push_back(Vertex {{x + precision, static_cast<Float> (z3), y + precision}, color});
		}
	}

	// Transform from normalized to -1 +1
	for (Vertex &vertex : vertices) {
		vertex.position *= 2;
		vertex.position -= Vector3(1.0f, 1.0f, 1.0f);
	}
    
	buffer.setData(vertices, GL::BufferUsage::StaticDraw);

	Debug{} << "Vertices:" << vertices.size();
	Debug{} << "Triangles:" << vertices.size() / 3;

	this->mesh.setPrimitive(GL::MeshPrimitive::Triangles)
		.setCount(vertices.size())
		.addVertexBuffer(buffer,
			0,
			Shaders::VertexColorGL3D::Position{},
			Shaders::VertexColorGL3D::Color4{}
		);
}
