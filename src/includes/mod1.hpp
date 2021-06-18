#ifndef _MOD1_HPP_
#define _MOD1_HPP_

#include <Magnum/Magnum.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Platform/Sdl2Application.h>

#include "terrainDefinition.hpp"

using namespace Magnum;

class Mod1: public Platform::Application {
    public:
        explicit Mod1(const Arguments& arguments);

    private:
        void	drawEvent() override;
        void	mouseMoveEvent(MouseMoveEvent& event) override;
		void	mouseScrollEvent(MouseScrollEvent &event) override;
		
		TerrainDefinition 			terrain;
		Shaders::VertexColorGL3D	shader;
		// Render matrices
		Matrix4 					translationMatrix;
		Matrix4 					rotationMatrix;
		Matrix4 					projectionMatrix;

};

MAGNUM_APPLICATION_MAIN(Mod1)

#endif
