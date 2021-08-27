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
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")

add_executable(mod1 sources/mod1.cpp
					sources/mod1Events.cpp
					sources/mod1Gui.cpp
					sources/mod1Parser.cpp
					sources/terrainDefinition.cpp
					sources/meshCreation.cpp
					sources/grid.cpp
					sources/cell.cpp
					sources/waterSimulation.cpp
					sources/scenarioHandler.cpp
					sources/scenarios/scenario.cpp
					sources/scenarios/waterRiseScenario.cpp
					sources/scenarios/waterSpreadScenario.cpp
					sources/scenarios/waveScenario.cpp
					sources/scenarios/rainScenario.cpp
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
