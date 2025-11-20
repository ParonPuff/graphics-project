#pragma once
#include "../core/renderer.hpp"
#include "../core/window.hpp"
#include "core/input.hpp"
#include <obj_loader.hpp>
#include <types.hpp>

class App : public InputHandler{
public:
  App();
  void run();
  void loadObject(const std::string &filename);
  
  void onKey(int key,int scancode, int action, int mods) override;
private:
  Window m_window;
  Renderer m_renderer;
  matvec::mat4 m_modelMatrix;
};
