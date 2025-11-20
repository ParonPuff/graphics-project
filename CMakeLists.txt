cmake_minimum_required(VERSION 3.16)
project(Studio3D LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add our libraries
add_subdirectory(lib/matvec)
add_subdirectory(lib/object_loader)
# Engine / App sources
file(GLOB_RECURSE ENGINE_SRC
    src/core/*.cpp
    src/app/*.cpp
)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(GLEW REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(studio3d
  src/main.cpp
  ${ENGINE_SRC}
)

target_include_directories(studio3d PRIVATE 
  src
  ${GLFW_INCLUDE_DIRS}
)

target_link_libraries(studio3d
  PRIVATE
    matvec
    objloader
    OpenGL::GL
    ${GLFW_LIBRARIES}
    GLEW::GLEW
)
