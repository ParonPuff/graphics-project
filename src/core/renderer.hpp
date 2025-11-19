class Renderer {
  public:
    Renderer();
    ~Renderer();

    void loadMesh(const Mesh& m);
    void setModelMatrix(const Mat4& M);
    void draw();
    void setWireframe(bool enabled);

  private:
    GLuint VAO, VBO, EBO;
    GLuint shader;
    Mat4 model;
}
