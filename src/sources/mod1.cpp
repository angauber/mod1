#include "mod1.hpp"

Mod1::Mod1(const Arguments& arguments): Platform::Application{arguments}
{
	Color4 backgroundColor (50.0 / 255, 50.0 / 255, 50.0 / 255, 1);

	GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
//	GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

	GL::Renderer::setClearColor(backgroundColor);

	// X is horizontal position, Y the elevation and Z the depth
	this->translationMatrix = Matrix4::translation(Vector3 {0.0f, 0.0f, -5.0f});
	this->projectionMatrix = Matrix4::perspectiveProjection(Deg{35.0f}, 1.0f, 0.0001f, 100.0f);


	this->terrain.addPoint(Vector3 {10000,10000,6000});
//	this->terrain.addPoint(Vector3 {15000,10000,100});
	this->terrain.addPoint(Vector3 {15000,15000,4000});

	this->terrain.computeEdges();
	this->terrain.scale();
	this->terrain.computeMesh();
}

void Mod1::mouseScrollEvent(MouseScrollEvent &event)
{
	float delta {event.offset().y()};
	Vector3 translation {0.0f, 0.0f, delta};

	this->translationMatrix = this->translationMatrix * Matrix4::translation(translation * 0.3f);

    event.setAccepted();
    this->redraw();
}

void Mod1::mouseMoveEvent(MouseMoveEvent &event)
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

    event.setAccepted();
    this->redraw();
}

void	Mod1::drawEvent()
{
	GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

	this->shader.setTransformationProjectionMatrix(projectionMatrix * this->translationMatrix * this->rotationMatrix)
        .draw(this->terrain.mesh);

	this->swapBuffers();
}
