#ifndef _MOD1_HPP_
#define _MOD1_HPP_

#include <vector>
#include <ranges>

#include <Magnum/Magnum.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Timeline.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include "scenarioHandler.hpp"
#include "terrainDefinition.hpp"
#include "waterSimulation.hpp"

using namespace Magnum;

class Mod1 : public Platform::Application, public ScenarioHandler
{
    public:
        explicit Mod1(const Arguments& arguments);

    private:
		void	setupRendering();
        void	drawEvent() override;
		void	drawGUI();

		void	viewportEvent(ViewportEvent &event) override;
        void	keyPressEvent(KeyEvent &event) override;
        void	keyReleaseEvent(KeyEvent &event) override;
        void	mousePressEvent(MouseEvent &event) override;
        void	mouseReleaseEvent(MouseEvent &event) override;
        void	mouseMoveEvent(MouseMoveEvent &event) override;
		void	mouseScrollEvent(MouseScrollEvent &event) override;
		void	textInputEvent(TextInputEvent &event) override;

		bool						running {true};
		ImGuiIntegration::Context	imgui {NoCreate};

		Timeline					timeline;
		float						timeToUpdate {0.0f};
		const float					timeStep = {1.0f / 60.0f};

		float						fpsTime {0.0f};
		int							fpsCounter {0};
		int							fps {0};

		GL::Mesh					terrainMesh;
		GL::Mesh					waterMesh;

		Shaders::PhongGL			shader {Shaders::PhongGL::Flag::VertexColor};

		Matrix4 					translationMatrix;
		Matrix4 					rotationMatrix;
		Matrix4 					transformationMatrix;
		Matrix4 					projectionMatrix;

};

#endif
