#include <Scene_TexturedTriangle.h>

#include <Engine.h>

#include <stb_image.h>

const std::string SceneTexturedTriangle::gSceneName = "Textured Triangle";

const std::string &SceneTexturedTriangle::getName() const { return gSceneName; }

bool SceneTexturedTriangle::init() {
  return initShaders() && initGeometry() && initTextures();
}

void SceneTexturedTriangle::onUpdate(float _delta) {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(m_Program.getHandle());
  m_Program.setUniform("uTexture", 0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_Texture);

  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
}

void SceneTexturedTriangle::onShutdown() {}

bool SceneTexturedTriangle::initShaders() {
  const std::string vsSrc =
      FILE_SYSTEM.readTextFromFile("shaders/scene_textured_triangle.vs");

  if (vsSrc.empty()) {
    //#TODO: Log
    return false;
  }

  const std::string fsSrc =
      FILE_SYSTEM.readTextFromFile("shaders/scene_textured_triangle.fs");

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

bool SceneTexturedTriangle::initGeometry() {
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

  GLfloat triangleVtx[] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, -0.5f,
                           0.0f,  1.0f,  0.0f, 0.5f, 0.5f, 1.0f};

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVtx), triangleVtx,
               GL_STATIC_DRAW);

  GLuint triangleIdx[] = {0, 1, 2};

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIdx), triangleIdx,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
                        (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4,
                        (void *)(sizeof(GLfloat) * 2));

  m_VAO = vao;
  m_VBO = vbo;
  m_EBO = ebo;

  return true;
}

bool SceneTexturedTriangle::initTextures() {
  GLuint texture = INVALID_GL_HANDLE;
  glGenTextures(1, &texture);

  if (texture == INVALID_GL_HANDLE) {
    LOGGER.log("Unnable to create texture object");

    return false;
  }

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  const std::vector<unsigned char> buffer =
      FILE_SYSTEM.readBinaryFromFile("textures/moss.jpg");

  if (buffer.empty()) {
    LOGGER.log("Unnable to read texture");

    return false;
  }

  int width = 0;
  int height = 0;
  int channels = 0;

  stbi_uc *textureData = stbi_load_from_memory(buffer.data(), buffer.size(),
                                               &width, &height, &channels, 0);

  if (!textureData) {
    LOGGER.log("Unnable to process texture data");

    return false;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, textureData);

  stbi_image_free(textureData);
  m_Texture = texture;

  return true;
}

// ---------- Factory ----------

const std::string &SceneTexturedTriangleFactory::getName() const {
  return SceneTexturedTriangle::gSceneName;
}

std::unique_ptr<Scene> SceneTexturedTriangleFactory::instantiate() {
  std::unique_ptr<SceneTexturedTriangle> scene{new SceneTexturedTriangle{}};

  if (scene->init()) {
    return std::move(scene);
  }

  return nullptr;
}