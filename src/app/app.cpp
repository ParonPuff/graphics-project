#include "app.hpp"
#include "../core/projection.hpp"
#include "ImGuiFileDialog.h"
#include "core/camera.hpp"
#include "core/renderer.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <math.h>
#include <string>

App::App() : m_window(800, 600, "Studio 3D") {
  m_modelMatrix = matvec::mat4::identity();
  m_window.setInputHandler(this);

  fov = 60.0f;  // Standard field of view
  farZ = 50.0f; // Far clipping plane
  top = 2.0f;   // Orthographic/oblique top bound
  a = 0.5f;     // Oblique shear strength
  phi = 45.0f;  // Oblique angle in degrees

  updateProjection(0);
}

void App::loadObject(const std::string &filename) {
  ObjLoader loader;
  Mesh mesh = loader.load(filename);

  m_modelMatrix = matvec::mat4::identity();

  m_renderer.loadMesh(mesh);
}

void App::drawGui() {
  IM_ASSERT(ImGui::GetCurrentContext() != NULL &&
            "Missing dear imgui context.");

  // // Change these variables to be class variables instead of static
  // // and delete the static declarations below
  // static std::string objFileName;
  // static std::string objFilePath;
  //
  // static ImGuiSliderFlags flags = ImGuiSliderFlags_AlwaysClamp;
  // static ImGuiFileDialog fileDialog;
  //
  // ImGui::Begin("3D Studio");
  //
  // if (ImGui::CollapsingHeader("OBJ File")) {
  //   ImGui::Text("OBJ file: %s", objFileName.c_str());
  //   if (ImGui::Button("Open File"))
  //     fileDialog.OpenDialog("ChooseFileDlgKey", "Choose File", ".obj", ".");
  //
  //   if (fileDialog.Display("ChooseFileDlgKey")) {
  //     if (fileDialog.IsOk() == true) {
  //       objFileName = fileDialog.GetCurrentFileName();
  //       objFilePath = fileDialog.GetCurrentPath();
  //       std::cout << "OBJ file: " << objFileName << std::endl
  //                 << "Path: " << objFilePath << std::endl;
  //
  //       loadObject(objFilePath);
  //     }
  //     fileDialog.Close();
  //   }
  // }
  //
  // if (ImGui::CollapsingHeader("Projection")) {
  //   const char *items[] = {"Perspective", "Parallel"};
  //   static int proj_current_idx = 0;
  //   if (ImGui::Combo("projektion", &proj_current_idx, items,
  //                    IM_ARRAYSIZE(items), IM_ARRAYSIZE(items)))
  //     ;
  //   if (proj_current_idx == 0) {
  //     ImGui::SliderFloat("Field of view", &fov, 20.0f, 160.0f, "%1.0f",
  //     flags); ImGui::SliderFloat("Far", &farZ, 1.0f, 1000.0f, "%1.0f",
  //     flags);
  //   }
  //   if (proj_current_idx == 1) {
  //     ImGui::SliderFloat("Top", &top, 1.0f, 100.0f, "%.1f", flags);
  //     ImGui::SliderFloat("Far", &farZ, 1.0f, 1000.0f, "%1.0f", flags);
  //     ImGui::SliderFloat("Oblique scale", &a, 0.0f, 1.0f, "%.1f", flags);
  //     ImGui::SliderAngle("Oblique angle", &phi, 15, 75, "%1.0f", flags);
  //   }
  // }
  //
  // ImGui::End();
  ImGui::Begin("Controls");

  // Projection selection
  static int projectionMode = 0; // 0=persp, 1=ortho, 2=oblique
  ImGui::RadioButton("Perspective", &projectionMode, 0);
  ImGui::RadioButton("Orthographic", &projectionMode, 1);
  ImGui::RadioButton("Oblique", &projectionMode, 2);

  // Common parameters
  ImGui::SliderFloat("Far Z", &farZ, 1.0f, 100.0f);

  if (projectionMode == 0) {
    // perspective
    ImGui::SliderFloat("FOV", &fov, 20.0f, 120.0f);
  }

  if (projectionMode == 1) {
    // orthographic
    ImGui::SliderFloat("Top", &top, 0.1f, 4.0f);
  }

  if (projectionMode == 2) {
    // oblique
    ImGui::SliderFloat("Top", &top, 0.1f, 4.0f);
    ImGui::SliderFloat("a (shear strength)", &a, 0.0f, 2.0f);
    ImGui::SliderFloat("Phi (deg)", &phi, -90.0f, 90.0f);
  }

  // Apply projection
  updateProjection(projectionMode);

  // File dialog
  if (ImGui::Button("Load OBJ")) {
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                            ".obj", ".");
  }

  // Show file dialog
  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filepath = ImGuiFileDialog::Instance()->GetFilePathName();
      loadObject(filepath); // your loader
    }
    ImGuiFileDialog::Instance()->Close();
  }

  ImGui::End();
}

void App::updateProjection(int mode) {
  int w, h;
  glfwGetFramebufferSize(m_window.raw(), &w, &h);
  float aspect = (float)w / (float)h;
  matvec::mat4 P;
  if (mode == 0) {
    P = makePerspective(fov, aspect, 0.1f, farZ);
  } else if (mode == 1) {
    float bottom = -top;
    float right = top * aspect;
    float left = -right;
    P = makeOrthographic(left, right, bottom, top, 0.1f, farZ);
  } else if (mode == 2) {
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    matvec::mat4 ortho = makeOrthographic(left, right, bottom, top, 0.1f, farZ);
    P = makeOblique(ortho, a, phi);
  }

  m_renderer.setProjectionMatrix(P);
}

void App::run() {
  static int currentProjectionMode = 0; // 0=persp, 1=ortho, 2=oblique
  while (!m_window.shouldClose()) {
    m_window.pollEvents();

    // Start ImGui frame FIRST (before any OpenGL calls)
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    drawGui(); // Build GUI (doesn't render yet)

    // NOW do OpenGL rendering
    // updateProjection(0); // or whatever mode you need
    m_renderer.setModelMatrix(m_modelMatrix);
    m_renderer.setViewMatrix(camera.getViewMatrix());
    m_renderer.draw(); // Draw 3D scene

    ImGui::Render(); // Finalize GUI (doesn't render yet)
    // Draw ImGui on top
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_window.swapBuffers();
    glfwWaitEvents();
  }
}

void App::onKey(int key, int scancode, int action, int mods) {
	/*if (action != GLFW_PRESS && action != GLFW_REPEAT)*/
	/*	return;*/

	const float moveSpeed = 0.1f;

	switch (key) {
		// camera movement
		case GLFW_KEY_W:
			camera.moveLocal(0, 0, -moveSpeed);
			break;
		case GLFW_KEY_A:
			camera.moveLocal(-moveSpeed, 0, 0);
			break;
		case GLFW_KEY_S:
			camera.moveLocal(0, 0, moveSpeed);
			break;
		case GLFW_KEY_D:
			camera.moveLocal(moveSpeed, 0, 0);
			break;
		case GLFW_KEY_E:
			camera.moveLocal(0, moveSpeed, 0);
			break;
		case GLFW_KEY_Q:
			camera.moveLocal(0, -moveSpeed, 0);
			break;
		// model transformations
		case GLFW_KEY_J:
			m_modelMatrix = m_modelMatrix * matvec::mat4::translate(-0.1, 0, 0);
			break;
		case GLFW_KEY_L:
			m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0.1, 0, 0);
			break;
		case GLFW_KEY_I:
			m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0, 0.1, 0);
			break;
		case GLFW_KEY_K:
			m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0, -0.1, 0);
			break;
		case GLFW_KEY_UP:
			m_modelMatrix = m_modelMatrix * matvec::mat4::rotateX(.17f);
			break;
		case GLFW_KEY_DOWN:
			m_modelMatrix = m_modelMatrix * matvec::mat4::rotateX(-.17f);
			break;
		case GLFW_KEY_LEFT:
			m_modelMatrix = m_modelMatrix * matvec::mat4::rotateY(.17f);
			break;
		case GLFW_KEY_RIGHT:
			m_modelMatrix = m_modelMatrix * matvec::mat4::rotateY(-.17f);
			break;
		case GLFW_KEY_LEFT_SHIFT:		
			if (action == GLFW_PRESS) {
				m_mouseRotating = true;
				m_firstMouse = true; // Reset on new click
			} else if (action == GLFW_RELEASE) {
				m_mouseRotating = false;
			}
			
			break;
		case GLFW_KEY_O:
			std::cout << "Enter object file: ";
			std::string file;
			std::cin >> file;
			loadObject(file);
			break;
	}
}

void App::onMouseButton(int button, int action, int mods) {
  /*if (button == GLFW_MOUSE_BUTTON_LEFT) {*/
  /*  if (action == GLFW_PRESS) {*/
  /*    m_mouseRotating = true;*/
  /*    m_firstMouse = true; // Reset on new click*/
  /*  } else if (action == GLFW_RELEASE) {*/
  /*    m_mouseRotating = false;*/
  /*  }*/
  /*}*/
}

void App::onMouseMove(double xpos, double ypos) {
  // Only rotate if mouse button is held down
  if (!m_mouseRotating) {
    return;
  }

  // Initialize last position on first movement
  if (m_firstMouse) {
    m_lastMouseX = xpos;
    m_lastMouseY = ypos;
    m_firstMouse = false;
    return;
  }

  // Calculate mouse movement delta
  double deltaX = xpos - m_lastMouseX;
  double deltaY = ypos - m_lastMouseY;

  m_lastMouseX = xpos;
  m_lastMouseY = ypos;

  // Sensitivity factor (adjust as needed)
  const float sensitivity = 0.005f;

  // Yaw rotation (left/right mouse movement rotates around Y axis)
  if (deltaX != 0) {
    camera.rotateYaw(-deltaX * sensitivity);
  }

  // Pitch rotation (up/down mouse movement rotates around X axis)
  if (deltaY != 0) {
    camera.rotatePitch(-deltaY * sensitivity);
  }
}
