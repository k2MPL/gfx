#pragma once

#include <Scene.h>
#include <gfx/GFXShader.h>

class SceneTriangle : public Scene {
public:
  bool init();

  static const std::string gSceneName;
  const std::string &getName() const override;

private:
  void onUpdate(float _delta);
  void onShutdown();

  bool initShaders();
  bool initGeometry();

private:
  GFXShaderProgram m_Program;

  GLuint m_VAO{INVALID_GL_HANDLE};
  GLuint m_VBO{INVALID_GL_HANDLE};
  GLuint m_EBO{INVALID_GL_HANDLE};
};

class SceneTriangleFactory : public SceneFactory {
private:
  const std::string &getName() const override;
  std::unique_ptr<Scene> instantiate() override;
};