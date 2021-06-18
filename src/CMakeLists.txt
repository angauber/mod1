find_package(Magnum REQUIRED
	Sdl2Application
	GL
	Primitives
	Shaders
)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -g3")

add_executable(mod1 sources/mod1.cpp
					sources/landDefinition.cpp
)

target_link_libraries(mod1 PRIVATE
    Magnum::Application
    Magnum::GL
    Magnum::Magnum
    Magnum::Shaders
	Magnum::Primitives
	Magnum::Shaders
)

target_include_directories(mod1 PRIVATE includes)
