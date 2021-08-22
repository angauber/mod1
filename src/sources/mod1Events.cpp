#include "mod1.hpp"

void	Mod1::viewportEvent(ViewportEvent &event)
{
	GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
	this->imgui.relayout(Vector2{event.windowSize()}/event.dpiScaling(), event.windowSize(), event.framebufferSize());
}

void	Mod1::keyPressEvent(KeyEvent &event)
{
	if (this->imgui.handleKeyPressEvent(event))	return;
}

void	Mod1::keyReleaseEvent(KeyEvent &event)
{
	if (this->imgui.handleKeyReleaseEvent(event)) return;

	switch (event.key()) {
		case KeyEvent::Key::Esc:
			event.setAccepted();
			this->exit();
			break;
		default:
			event.setAccepted(false);
			break;
	}
}

void	Mod1::mousePressEvent(MouseEvent &event)
{
	if (this->imgui.handleMousePressEvent(event)) return;
}

void	Mod1::mouseReleaseEvent(MouseEvent &event)
{
	if (this->imgui.handleMouseReleaseEvent(event))	return;
}

void	Mod1::mouseMoveEvent(MouseMoveEvent &event)
{
	if (this->imgui.handleMouseMoveEvent(event) || !(event.buttons() & MouseMoveEvent::Button::Left || event.buttons() & MouseMoveEvent::Button::Right)) return;

	Vector2 delta = Vector2 {event.relativePosition()} / Vector2 {this->windowSize()} * 3.0f;

	if (event.buttons() & MouseMoveEvent::Button::Left) {
		/**
		 * The X axis on screen (Horizontal) is matched with the Y axis in the world (Elevation)
		 * The Y axis on screen (Vertical) is matched with the X axis in the world (Horizontal)
		 * */
		this->rotationMatrix = Matrix4::rotationX(Rad {delta.y()}) * this->rotationMatrix * Matrix4::rotationY(Rad {delta.x()});
	}

	if (event.buttons() & MouseMoveEvent::Button::Right) {
		this->translationMatrix = this->translationMatrix * Matrix4::translation({delta.x(), delta.y() * -1.0f, 0.0f});
	}

	this->transformationMatrix = this->translationMatrix * this->rotationMatrix;
	event.setAccepted();
}

void	Mod1::mouseScrollEvent(MouseScrollEvent &event)
{
	if (this->imgui.handleMouseScrollEvent(event)) return;

	float delta {event.offset().y()};
	Vector3 translation {0.0f, 0.0f, delta};

	this->translationMatrix = this->translationMatrix * Matrix4::translation(translation * 0.3f);
	this->transformationMatrix = this->translationMatrix * this->rotationMatrix;

	event.setAccepted();
}

void	Mod1::textInputEvent(TextInputEvent &event)
{
    if (this->imgui.handleTextInputEvent(event)) return;
}
