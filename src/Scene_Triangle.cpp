#include <Engine.h>
#include <Scene_Triangle.h>

const std::string SceneTriangle::gSceneName = "Triangle";

bool SceneTriangle::init() { return initShaders() && initGeometry(); }

void SceneTriangle::onUpdate(float _delta) {
  glClearColor(1.0, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(m_Program.getHandle());

  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void SceneTriangle::onShutdown() {
  m_Program.shutdown();

  if (m_VAO != INVALID_GL_HANDLE)
    glDeleteVertexArrays(1, &m_VAO);

  if (m_VBO != INVALID_GL_HANDLE)
    glDeleteBuffers(1, &m_VBO);

  if (m_EBO != INVALID_GL_HANDLE)
    glDeleteBuffers(1, &m_EBO);
}

bool SceneTriangle::initShaders() {
  const std::string vsSrc =
      FILE_SYSTEM.readTextFromFile("shaders/scene_triangle.vs");

  if (vsSrc.empty()) {
    //#TODO: Log
    return false;
  }

  const std::string fsSrc =
      FILE_SYSTEM.readTextFromFile("shaders/scene_triangle.fs");

  if (fsSrc.empty()) {
    //#TODO: Log
    return false;
  }

  struct ShaderScope {
    GFXShader *shader{nullptr};

    ~ShaderScope() {
      if (shader) {
        shader->shutdown();
      }
    }
  };

  GFXShader::InitInfo vsInitInfo;
  vsInitInfo.type = GL_VERTEX_SHADER;
  vsInitInfo.src = vsSrc;

  GFXShader vs;

  ShaderScope vsScope;
  vsScope.shader = &vs;

  if (!vs.init(vsInitInfo)) {
    return false;
  }

  GFXShader::InitInfo fsInitInfo;
  fsInitInfo.type = GL_FRAGMENT_SHADER;
  fsInitInfo.src = fsSrc;

  GFXShader fs;

  ShaderScope fsScope;
  fsScope.shader = &fs;

  if (!fs.init(fsInitInfo)) {
    return false;
  }

  GFXShaderProgram::InitInfo programInitInfo;
  programInitInfo.shaders.push_back(&vs);
  programInitInfo.shaders.push_back(&fs);

  if (!m_Program.init(programInitInfo)) {
    return false;
  }

  return true;
}

bool SceneTriangle::initGeometry() {
  GLuint vao = INVALID_GL_HANDLE;
  glGenVertexArrays(1, &vao);

  if (vao == INVALID_GL_HANDLE) {
    LOGGER.log("Unnable to create VAO object");

    return false;
  }

  GLuint vbo = INVALID_GL_HANDLE;
  glGenBuffers(1, &vbo);

  if (vbo == INVALID_GL_HANDLE) {
    LOGGER.log("Unnable to create VBO object");

    return false;
  }

  GLuint ebo = INVALID_GL_HANDLE;
  glGenBuffers(1, &ebo);

  if (ebo == INVALID_GL_HANDLE) {
    LOGGER.log("Unnable to create EBO handle");

    return false;
  }

  glBindVertexArray(vao);

  GLfloat triangleVtx[] = {-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.0f,
                           1.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f, 1.0f};

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVtx), triangleVtx,
               GL_STATIC_DRAW);

  GLuint triangleIdx[] = {0, 1, 2};

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIdx), triangleIdx,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5,
                        (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5,
                        (void *)(sizeof(GLfloat) * 2));

  m_VAO = vao;
  m_VBO = vbo;
  m_EBO = ebo;

  return true;
}

const std::string &SceneTriangle::getName() const { return gSceneName; }

const std::string &SceneTriangleFactory::getName() const {
  return SceneTriangle::gSceneName;
}

std::unique_ptr<Scene> SceneTriangleFactory::instantiate() {
  std::unique_ptr<SceneTriangle> scene{new SceneTriangle{}};

  if (scene->init()) {
    return std::move(scene);
  }

  return nullptr;
}