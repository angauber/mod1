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

		this->setupRendering();
	}
}

void	Mod1::setupRendering()
{
	Color3 backgroundColor (50.0 / 255, 50.0 / 255, 50.0 / 255);

	// X is horizontal position, Y the elevation and Z the depth
	this->translationMatrix = Matrix4::translation(Vector3 {0.0f, 0.0f, -6.0f});
	this->transformationMatrix = this->translationMatrix * this->rotationMatrix;
	this->projectionMatrix = Matrix4::perspectiveProjection(Deg{35.0f}, 1.0f, 0.0001f, 100.0f);

	this->timeline.start();

	this->imgui = ImGuiIntegration::Context(Vector2 {windowSize()} / this->dpiScaling(), this->windowSize(), this->framebufferSize());

	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling); 
	GL::Renderer::enable(GL::Renderer::Feature::Blending);

	GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add, GL::Renderer::BlendEquation::Add);
	GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::SourceAlpha, GL::Renderer::BlendFunction::OneMinusSourceAlpha);

	GL::Renderer::setClearColor(backgroundColor);
}

void	Mod1::drawEvent()
{
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

	if (this->fpsTime >= 1.0f) {
		this->fps = this->fpsCounter;
		this->fpsCounter = 0;
		this->fpsTime = 0.0f;
	}

	if (this->running && this->timeToUpdate > this->timeStep) {
		this->updateSimulation(this->timeStep);
		this->timeToUpdate = 0.0;
	}

	this->timeToUpdate += this->timeline.previousFrameDuration();
	this->fpsTime += this->timeline.previousFrameDuration();
	this->fpsCounter++;

	this->shader
		.setShininess(200.0f)
		.setTransformationMatrix(this->transformationMatrix)
		.setProjectionMatrix(this->projectionMatrix);

	GL::Renderer::setFaceCullingMode(GL::Renderer::PolygonFacing::Back);
	this->shader.setNormalMatrix(this->transformationMatrix.normalMatrix());

	this->shader.draw(this->terrainMesh);
	this->shader.draw(std::move(this->computeWaterMesh()));

	GL::Renderer::setFaceCullingMode(GL::Renderer::PolygonFacing::Front);
	this->shader.setNormalMatrix(this->transformationMatrix.normalMatrix() * -1.0f);

	this->shader.draw(this->terrainMesh);
	this->shader.draw(std::move(this->computeWaterMesh()));

	this->drawGUI();

	this->swapBuffers();
	this->redraw();
	this->timeline.nextFrame();
}

MAGNUM_APPLICATION_MAIN(Mod1)
