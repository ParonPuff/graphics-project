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
  m_renderer.setModelMatrix(m_modelMatrix);
  m_renderer.setViewMatrix(camera.getViewMatrix());
  m_window.setInputHandler(this);

  fov = 60.0f;  // Standard field of view
  farZ = 50.0f; // Far clipping plane
  top = 2.0f;   // Orthographic/oblique top bound
  a = 0.5f;     // Oblique shear strength
  phi = 45.0f;  // Oblique angle in degrees

  glfwSetFramebufferSizeCallback(
      m_window.raw(), [](GLFWwindow *win, int w, int h) {
        auto *self = static_cast<App *>(glfwGetWindowUserPointer(win));
        glViewport(0, 0, w, h);
        self->updateProjection();
      });

  updateProjection();
  // set to default values
  m_renderer.setLightPos(lightPos);
  m_renderer.setLightColor(lightColor);
  m_renderer.setAmbient(materialAmbient);
  m_renderer.setDiffuse(materialDiffuse);
  m_renderer.setSpecular(materialSpecular);
  m_renderer.setShininess(shininess);
  m_renderer.setUseTexture(useTexture);
}

void App::loadObject(const std::string &filename) {
  ObjLoader loader;
  Mesh mesh = loader.load(filename);

  m_modelMatrix = matvec::mat4::identity();
  m_renderer.setModelMatrix(m_modelMatrix);

  m_renderer.loadMesh(mesh);
}

void App::drawGui() {
  IM_ASSERT(ImGui::GetCurrentContext() != NULL &&
            "Missing dear imgui context.");

  ImGui::Begin("Controls");

  bool projectionChanged = false;
  projectionChanged |= ImGui::RadioButton("Perspective", &projectionMode, 0);
  projectionChanged |= ImGui::RadioButton("Orthographic", &projectionMode, 1);
  projectionChanged |= ImGui::RadioButton("Oblique", &projectionMode, 2);

  projectionChanged |= ImGui::SliderFloat("Far Z", &farZ, 1.0f, 100.0f);

  if (projectionMode == 0) {
    projectionChanged |= ImGui::SliderFloat("FOV", &fov, 20.0f, 120.0f);
  }

  if (projectionMode == 1) {
    projectionChanged |= ImGui::SliderFloat("Top", &top, 0.1f, 4.0f);
  }

  if (projectionMode == 2) {
    projectionChanged |= ImGui::SliderFloat("Top", &top, 0.1f, 4.0f);
    projectionChanged |=
        ImGui::SliderFloat("a (shear strength)", &a, 0.0f, 2.0f);
    projectionChanged |= ImGui::SliderFloat("Phi (deg)", &phi, -90.0f, 90.0f);
  }

  if (projectionChanged)
    updateProjection();

  if (ImGui::SliderFloat3("Light position", &lightPos.x, -10.0f, 10.0f))
    m_renderer.setLightPos(lightPos);

  if (ImGui::ColorEdit3("Light color", &lightColor.x))
    m_renderer.setLightColor(lightColor);

  if (ImGui::ColorEdit3("Ambient", &materialAmbient.x))
    m_renderer.setAmbient(materialAmbient);

  if (ImGui::ColorEdit3("Diffuse", &materialDiffuse.x))
    m_renderer.setDiffuse(materialDiffuse);

  if (ImGui::ColorEdit3("Specular", &materialSpecular.x))
    m_renderer.setSpecular(materialSpecular);

  if (ImGui::SliderFloat("Shininess", &shininess, 1.0f, 128.0f))
    m_renderer.setShininess(shininess);

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
  if (ImGui::Checkbox("Use texture", &useTexture))
    m_renderer.setUseTexture(useTexture);

  ImGui::End();
}

void App::updateProjection() {
  int w, h;
  glfwGetFramebufferSize(m_window.raw(), &w, &h);
  float aspect = (float)w / (float)h;
  float bottom = -top;
  float right = top * aspect;
  float left = -right;

  matvec::mat4 P;
  if (projectionMode == 0) {
    P = makePerspective(fov, aspect, 0.1f, farZ);
  } else if (projectionMode == 1) {
    P = makeOrthographic(left, right, bottom, top, 0.1f, farZ);
  } else if (projectionMode == 2) {
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
    m_renderer.setViewPos(camera.getPosition());
    m_renderer.setViewMatrix(camera.getViewMatrix());
    break;
  case GLFW_KEY_A:
    camera.moveLocal(-moveSpeed, 0, 0);
    m_renderer.setViewPos(camera.getPosition());
    m_renderer.setViewMatrix(camera.getViewMatrix());
    break;
  case GLFW_KEY_S:
    camera.moveLocal(0, 0, moveSpeed);
    m_renderer.setViewPos(camera.getPosition());
    m_renderer.setViewMatrix(camera.getViewMatrix());
    break;
  case GLFW_KEY_D:
    camera.moveLocal(moveSpeed, 0, 0);
    m_renderer.setViewPos(camera.getPosition());
    m_renderer.setViewMatrix(camera.getViewMatrix());
    break;
  case GLFW_KEY_E:
    camera.moveLocal(0, moveSpeed, 0);
    m_renderer.setViewPos(camera.getPosition());
    m_renderer.setViewMatrix(camera.getViewMatrix());
    break;
  case GLFW_KEY_Q:
    camera.moveLocal(0, -moveSpeed, 0);
    m_renderer.setViewPos(camera.getPosition());
    m_renderer.setViewMatrix(camera.getViewMatrix());
    break;
  case GLFW_KEY_J:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(-0.1, 0, 0);
    m_renderer.setModelMatrix(m_modelMatrix);
    break;
  case GLFW_KEY_L:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0.1, 0, 0);
    m_renderer.setModelMatrix(m_modelMatrix);
    break;
  case GLFW_KEY_I:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0, 0.1, 0);
    m_renderer.setModelMatrix(m_modelMatrix);
    break;
  case GLFW_KEY_K:
    m_modelMatrix = m_modelMatrix * matvec::mat4::translate(0, -0.1, 0);
    m_renderer.setModelMatrix(m_modelMatrix);
    break;
  case GLFW_KEY_UP:
    m_modelMatrix = matvec::mat4::rotateX(.17f) * m_modelMatrix;
    m_renderer.setModelMatrix(m_modelMatrix);
    break;
  case GLFW_KEY_DOWN:
    m_modelMatrix = matvec::mat4::rotateX(-.17f) * m_modelMatrix;
    m_renderer.setModelMatrix(m_modelMatrix);
    break;
  case GLFW_KEY_LEFT:
    m_modelMatrix = matvec::mat4::rotateY(.17f) * m_modelMatrix;
    m_renderer.setModelMatrix(m_modelMatrix);
    break;
  case GLFW_KEY_RIGHT:
    m_modelMatrix = matvec::mat4::rotateY(-.17f) * m_modelMatrix;
    m_renderer.setModelMatrix(m_modelMatrix);
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
    camera.rotateYaw(deltaX * sensitivity);
    m_renderer.setViewMatrix(camera.getViewMatrix());
  }

  if (deltaY != 0) {
    camera.rotatePitch(deltaY * sensitivity);
    m_renderer.setViewMatrix(camera.getViewMatrix());
  }
}
