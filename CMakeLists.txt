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

# Add ImGui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/ImGui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/imgui_impl_opengl3.cpp
)

add_library(ImGui STATIC ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC
    ${IMGUI_DIR}
)
target_link_libraries(ImGui PUBLIC glfw OpenGL::GL)

# ImGuiFileDialog
set(IMGUI_FILE_DIALOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/ImGuiFileDialog)

add_library(ImGuiFileDialog STATIC
    ${IMGUI_FILE_DIALOG_DIR}/ImGuiFileDialog.cpp
)

target_include_directories(ImGuiFileDialog PUBLIC
    ${IMGUI_FILE_DIALOG_DIR}
    ${IMGUI_DIR}                # must see imgui.h
)
target_link_libraries(ImGuiFileDialog PUBLIC ImGui)


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
    ImGui
    ImGuiFileDialog
)
