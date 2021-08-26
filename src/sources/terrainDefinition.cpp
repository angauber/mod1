#include "terrainDefinition.hpp"

void	TerrainDefinition::debugTerrain() const
{
	for (const auto &point : this->points) {
		Debug{} << point;
	}
}

void	TerrainDefinition::handleTerrain()
{
	this->computeEdges();
	this->scale();
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

	for (auto &point : this->points) {
		point /= norm;
	}

	this->computeMinMax();

	/*
	 * Artificially adding null contours
	 **/
	size_t size {10};

	for (std::size_t i = 0; i < size; i++) {
		this->addPoint(Vector3 {0.0f, static_cast<float> (i) / size, 0.0f});
		this->addPoint(Vector3 {1.0f, static_cast<float> (i) / size, 0.0f});
		this->addPoint(Vector3 {static_cast<float> (i) / size, 0.0f, 0.0f});
		this->addPoint(Vector3 {static_cast<float> (i) / size, 1.0f, 0.0f});
	}
}

/**
 *  use IDW to interpolate coordinates
 */
float	TerrainDefinition::interpolateTerrain(float x, float y) const
{
	float power {3.0};
	float a {0.0f};
	float b {0.0f};
	float distance;
	float weight;

	std::vector<Vector3> neighbours = this->getClosestPoints(Vector3 {x, y, 0.0f}, 20);

	for (const auto &point : neighbours) {
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

std::vector<Vector3>	TerrainDefinition::getClosestPoints(Vector3 point, std::size_t nb) const
{
	std::vector<Vector3> points	= this->points;

	std::sort(points.begin(), points.end(), [point] (const Vector3 &a, const Vector3 &b) {
		float da = sqrt(pow(a.x() - point.x(), 2.0f) + pow(a.y() - point.y(), 2.0f));
		float db = sqrt(pow(b.x() - point.x(), 2.0f) + pow(b.y() - point.y(), 2.0f));

		return da < db;
	});

	points.resize(nb);

	return points;
}
