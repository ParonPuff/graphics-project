#include "app.hpp"
#include "../core/projection.hpp"
#include "ImGuiFileDialog.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

App::App() : m_window(800, 600, "Studio 3D") {
  m_modelMatrix = matvec::mat4::identity();
  m_window.setInputHandler(this);

  ImGui::CreateContext();

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(m_window.raw(), true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void App::loadObject(const std::string &filename) {
  ObjLoader loader;
  Mesh mesh = loader.load(filename);

  m_modelMatrix = matvec::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

  m_renderer.loadMesh(mesh);
}

void App::drawGui() {
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
  while (!m_window.shouldClose()) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    drawGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    m_window.pollEvents();

    m_renderer.setModelMatrix(m_modelMatrix);
    m_renderer.draw();

    m_window.swapBuffers();
  }
}
void App::onKey(int key, int scancode, int action, int mods) {
  if (action != GLFW_PRESS && action != GLFW_REPEAT)
    return;

  for (auto e : m_modelMatrix.data) {
    std::cout << e;
  }
  std::cout << '\n';
  switch (key) {
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
  case GLFW_KEY_O:
    std::cout << "Enter object file: ";
    std::string file;
    std::cin >> file;
    loadObject(file);
    break;
  }
}
