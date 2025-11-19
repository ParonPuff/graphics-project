class Window {
  public:
    Window(int width, int height, const char* title);
    ~Widnow();

    bool shouldClose() const;
    bool pollEvents() const;
    bool swapBuffers() const;

    GLFWwindow* raw();
  private:
    GLFWwindow* m_window;
};
