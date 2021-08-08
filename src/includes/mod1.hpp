#ifndef _MOD1_HPP_
#define _MOD1_HPP_

#include <vector>

#include <Magnum/Magnum.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Trade/MeshData.h>
#include <Magnum/Math/Matrix4.h>

#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/PhongGL.h>
#include <Magnum/Trade/MeshData.h>

#include <Magnum/Timeline.h>

#include "terrainDefinition.hpp"
#include "waterSimulation.hpp"

using namespace Magnum;

class Mod1 : public Platform::Application, public WaterSimulation
{
    public:
        explicit Mod1(const Arguments& arguments);

    private:
		void	createWaterMeshes();
		void	setupRendering();
		void	renderWater();
        void	keyReleaseEvent(KeyEvent &event) override;
        void	mouseMoveEvent(MouseMoveEvent &event) override;
		void	mouseScrollEvent(MouseScrollEvent &event) override;
        void	drawEvent() override;

		Timeline					timeline;
//		float						timeToUpdate = 1.0f; // 60 hz
		float						timeToUpdate = 1.0f / 60; // 60 hz
		float						updateTime = 0.0f;

		GL::Mesh					terrainMesh;
		GL::Mesh					waterMesh;

		Shaders::PhongGL			shader;

		Matrix4 					translationMatrix;
		Matrix4 					rotationMatrix;
		Matrix4 					transformationMatrix;
		Matrix4 					projectionMatrix;

};

MAGNUM_APPLICATION_MAIN(Mod1)

#endif
