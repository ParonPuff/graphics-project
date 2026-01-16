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

  ImGui::Begin("Controls");

  static int projectionMode = 0;
  ImGui::RadioButton("Perspective", &projectionMode, 0);
  ImGui::RadioButton("Orthographic", &projectionMode, 1);
  ImGui::RadioButton("Oblique", &projectionMode, 2);

  ImGui::SliderFloat("Far Z", &farZ, 1.0f, 100.0f);

  ImGui::Checkbox("Use texture", &useTexture);
  ImGui::SliderFloat3("Light position", &lightPos.x, -10.0f, 10.0f);
  ImGui::ColorEdit3("Light color", &lightColor.x);

  ImGui::ColorEdit3("Ambient", &materialAmbient.x);
  ImGui::ColorEdit3("Diffuse", &materialDiffuse.x);
  ImGui::ColorEdit3("Specular", &materialSpecular.x);
  ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f);

  if (projectionMode == 0) {
    ImGui::SliderFloat("FOV", &fov, 20.0f, 120.0f);
  }

  if (projectionMode == 1) {
    ImGui::SliderFloat("Top", &top, 0.1f, 4.0f);
  }

  if (projectionMode == 2) {
    ImGui::SliderFloat("Top", &top, 0.1f, 4.0f);
    ImGui::SliderFloat("a (shear strength)", &a, 0.0f, 2.0f);
    ImGui::SliderFloat("Phi (deg)", &phi, -90.0f, 90.0f);
  }

  updateProjection(projectionMode);

  if (ImGui::Button("Load OBJ")) {
    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File",
                                            ".obj", ".");
  }

  if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filepath = ImGuiFileDialog::Instance()->GetFilePathName();
      loadObject(filepath);
    }
    ImGuiFileDialog::Instance()->Close();
  }

  if (ImGui::Button("load Texture")) {
    ImGuiFileDialog::Instance()->OpenDialog("texKey", "Choose File", ".bmp",
                                            ".");
  }

  if (ImGuiFileDialog::Instance()->Display("texKey")) {
    if (ImGuiFileDialog::Instance()->IsOk()) {
      std::string filepath = ImGuiFileDialog::Instance()->GetFilePathName();
      m_renderer.loadTexture(filepath);
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
  static int currentProjectionMode = 0;
  while (!m_window.shouldClose()) {
    m_window.pollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    drawGui();

    m_renderer.setModelMatrix(m_modelMatrix);
    m_renderer.setViewMatrix(camera.getViewMatrix());

    m_renderer.setLightPos(lightPos);
    m_renderer.setLightColor(lightColor);
    m_renderer.setViewPos(camera.getPosition());

    m_renderer.setAmbient(materialAmbient);
    m_renderer.setDiffuse(materialDiffuse);
    m_renderer.setSpecular(materialSpecular);

    m_renderer.setShininess(shininess);

    m_renderer.setUseTexture(useTexture);
    m_renderer.draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_window.swapBuffers();
    glfwWaitEvents();
  }
}

void App::onKey(int key, int scancode, int action, int mods) {

  const float moveSpeed = 0.1f;

  switch (key) {
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
      m_firstMouse = true;
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

void App::onMouseMove(double xpos, double ypos) {
  if (!m_mouseRotating) {
    return;
  }

  if (m_firstMouse) {
    m_lastMouseX = xpos;
    m_lastMouseY = ypos;
    m_firstMouse = false;
    return;
  }

  double deltaX = xpos - m_lastMouseX;
  double deltaY = ypos - m_lastMouseY;

  m_lastMouseX = xpos;
  m_lastMouseY = ypos;

  const float sensitivity = 0.005f;

  if (deltaX != 0) {
    camera.rotateYaw(-deltaX * sensitivity);
  }

  if (deltaY != 0) {
    camera.rotatePitch(-deltaY * sensitivity);
  }
}
