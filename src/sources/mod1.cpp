#include "mod1.hpp"

Mod1::Mod1(const Arguments& arguments): Platform::Application{arguments}
{
	if (arguments.argc != 2) {
		Error{} << "Missing mod1 file";
		this->exit(1);
	} else if (this->parseFile(arguments.argv[1]) == false) {
		Error{} << "No points have been parsed correctly";
		this->exit(2);
	} else {
		WaterSimulation::handleTerrain();

		this->terrainMesh = this->computeTerrainMesh();
//		this->terrainMesh = this->computeTerrainMesh();

//		this->createWaterMeshes();
		this->setupRendering();
	}
}

void	Mod1::createWaterMeshes()
{	
	GL::Buffer indices;
	GL::Buffer vertices;
	Trade::MeshData cube = Primitives::cubeSolid();

	std::pair<Containers::Array<char>, MeshIndexType> compressed = MeshTools::compressIndices(cube.indicesAsArray());

	indices.setData(compressed.first);

	auto positions = cube.positions3DAsArray();

	for (auto &position : positions) {
		position *= Vector3 {this->precision, 1.0f, this->precision};
	}

	vertices.setData(MeshTools::interleave(positions, cube.normalsAsArray()));

	this->waterMesh.setPrimitive(cube.primitive())
		.setCount(cube.indexCount())
		.addVertexBuffer(std::move(vertices), 0, Shaders::PhongGL::Position{}, Shaders::PhongGL::Normal{})
		.setIndexBuffer(std::move(indices), 0, compressed.second);
}

void	Mod1::setupRendering()
{
	Color3 backgroundColor (50.0 / 255, 50.0 / 255, 50.0 / 255);

	// X is horizontal position, Y the elevation and Z the depth
	this->translationMatrix = Matrix4::translation(Vector3 {0.0f, 0.0f, -6.0f});
	this->transformationMatrix = this->translationMatrix * this->rotationMatrix;
	this->projectionMatrix = Matrix4::perspectiveProjection(Deg{35.0f}, 1.0f, 0.0001f, 100.0f);

	this->timeline.start();

	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

	GL::Renderer::setClearColor(backgroundColor);
}

/**
 * Moving the transformation matrix around and drawing multiple times the same mesh (water column)
 */
void	Mod1::renderWater()
{
	SimulationGrid grid = this->getSimulationGrid();
	Trade::MeshData cube = Primitives::cubeSolid();
	Matrix4 transformation = this->transformationMatrix;

	for (size_t i = 0; i < grid.size(); i++) {
		for (size_t j = 0; j < grid.size(); j++) {
			if (grid[i][j].waterDepth > 0.000001f) {
				this->shader.setTransformationMatrix(
					transformation *
//					Matrix4::translation({((i * 0.01 * 2) - 1), -1, ((j * 0.01 * 2) - 1)})
					Matrix4::translation({((i * 0.01 * 2) - 1.0f), grid[i][j].terrainHeight + grid[i][j].waterDepth - 1.0f, ((j * 0.01 * 2) - 1.0f)})
					*
					Matrix4::scaling({1.0f, 0.01f, 1.0f})
				);

				this->shader.draw(this->waterMesh);
			}
		}
	}
	
	this->shader.setTransformationMatrix(transformation);
}

void	Mod1::mouseScrollEvent(MouseScrollEvent &event)
{
	float delta {event.offset().y()};
	Vector3 translation {0.0f, 0.0f, delta};

	this->translationMatrix = this->translationMatrix * Matrix4::translation(translation * 0.3f);
	this->transformationMatrix = this->translationMatrix * this->rotationMatrix;

	event.setAccepted();
}

void	Mod1::keyReleaseEvent(KeyEvent &event)
{
	if (event.key() == KeyEvent::Key::Esc) {
		event.setAccepted();
		this->exit();
	}
}

void	Mod1::mouseMoveEvent(MouseMoveEvent &event)
{
	if (!(event.buttons() & MouseMoveEvent::Button::Left)) {
		return;
	}

	Vector2 delta = Vector2 {event.relativePosition()} / Vector2 {this->windowSize()} * 3.0f;

	/**
	 * The X axis on screen (Horizontal) is matched with the Y axis in the world (Elevation)
	 * The Y axis on screen (Vertical) is matched with the X axis in the world (Horizontal)
	 * */
	this->rotationMatrix = Matrix4::rotationX(Rad {delta.y()}) * this->rotationMatrix * Matrix4::rotationY(Rad {delta.x()});
	this->transformationMatrix = this->translationMatrix * this->rotationMatrix;

	event.setAccepted();
}

void	Mod1::drawEvent()
{
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
	Color3 color (234.0f / 255, 255.0f / 255, 0.0f / 255);

	this->shader
		.setDiffuseColor(color)
		.setShininess(200.0f)
		.setTransformationMatrix(this->transformationMatrix)
		.setNormalMatrix(this->transformationMatrix.normalMatrix())
		.setProjectionMatrix(this->projectionMatrix);

//	this->updateTime += this->timeline.previousFrameDuration();
//	if (this->updateTime >= this->timeToUpdate) {
		this->updateSimulation(this->timeline.previousFrameDuration());
//		this->updateSimulation(this->timeToUpdate);
//		this->updateTime = 0.0f;
//	}

	this->shader.draw(this->terrainMesh);
	this->shader.setDiffuseColor(Color3 {33.0f / 255, 92.0f / 255, 255.0f / 255});
	this->shader.draw(std::move(this->computeWaterMesh()));
	//this->renderWater();

	this->swapBuffers();
	this->redraw();
	this->timeline.nextFrame();
}
