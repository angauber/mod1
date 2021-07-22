#include "mod1.hpp"

Mod1::Mod1(const Arguments& arguments): Platform::Application{arguments}
{
	if (arguments.argc != 2) {
		Error{} << "Missing mod1 file";
		this->exit(1);
	} else if (this->parseFile(arguments.argv[1]) == false) {
		Error{} << "No points have been parsed correctly";
		this->exit(1);
	} else {
		this->computeEdges();
		this->scale();

		this->mesh = this->computeMesh();

		this->setupRendering();
	}
}

void	Mod1::setupRendering()
{
	Color3 backgroundColor (50.0 / 255, 50.0 / 255, 50.0 / 255);

	// X is horizontal position, Y the elevation and Z the depth
	this->translationMatrix = Matrix4::translation(Vector3 {0.0f, 0.0f, -6.0f});
	this->projectionMatrix = Matrix4::perspectiveProjection(Deg{35.0f}, 1.0f, 0.0001f, 100.0f);

	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
	GL::Renderer::setClearColor(backgroundColor);
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

	this->shader.setDiffuseColor(color)
		.setShininess(200.0f)
    	.setTransformationMatrix(this->transformationMatrix)
		.setNormalMatrix(this->transformationMatrix.normalMatrix())
		.setProjectionMatrix(this->projectionMatrix)
		.draw(this->mesh);

	this->swapBuffers();
	this->redraw();
}
