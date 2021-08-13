#ifndef _MOD1_HPP_
#define _MOD1_HPP_

#include <vector>

#include <Magnum/Magnum.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Timeline.h>

#include "terrainDefinition.hpp"
#include "waterSimulation.hpp"

using namespace Magnum;

class Mod1 : public Platform::Application, public WaterSimulation
{
    public:
        explicit Mod1(const Arguments& arguments);

    private:
		void	setupRendering();
        void	keyReleaseEvent(KeyEvent &event) override;
        void	mouseMoveEvent(MouseMoveEvent &event) override;
		void	mouseScrollEvent(MouseScrollEvent &event) override;
        void	drawEvent() override;

		Timeline					timeline;

		GL::Mesh					terrainMesh;
		GL::Mesh					waterMesh;

		Shaders::PhongGL			shader {Shaders::PhongGL::Flag::VertexColor};

		Matrix4 					translationMatrix;
		Matrix4 					rotationMatrix;
		Matrix4 					transformationMatrix;
		Matrix4 					projectionMatrix;

};

MAGNUM_APPLICATION_MAIN(Mod1)

#endif
