find_package(MagnumIntegration REQUIRED
	ImGui
)

find_package(Magnum REQUIRED
	Sdl2Application
	GL
	Primitives
	Shaders
)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER "/usr/bin/g++-10")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")

add_executable(mod1 sources/mod1.cpp
					sources/mod1Events.cpp
					sources/mod1Gui.cpp
					sources/mod1Parser.cpp
					sources/terrainDefinition.cpp
					sources/meshCreation.cpp
					sources/grid.cpp
					sources/waterSimulation.cpp
					sources/scenario.cpp
					sources/waterRiseScenario.cpp
					sources/waterSpreadScenario.cpp
					sources/waveScenario.cpp
					sources/rainScenario.cpp
)

target_link_libraries(mod1 PRIVATE
    Magnum::Application
	Magnum::GL
    Magnum::Magnum
    Magnum::Shaders
	Magnum::Primitives
	Magnum::Shaders
	MagnumIntegration::ImGui
)

target_include_directories(mod1 PRIVATE includes)
