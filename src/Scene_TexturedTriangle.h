#pragma once

#include <Scene.h>

#include <gfx/GFXShader.h>
#include <gfx/GFXTexture2D.h>

class SceneTexturedTriangle : public Scene {
public:
  bool init();

  static const std::string gSceneName;
  const std::string &getName() const override;

private:
  void onUpdate(float _delta) override;
  void onShutdown() override;

  bool initShaders();
  bool initGeometry();
  bool initTextures();

private:
  GFXShaderProgram m_Program;

  GLuint m_VAO{INVALID_GL_HANDLE};
  GLuint m_VBO{INVALID_GL_HANDLE};
  GLuint m_EBO{INVALID_GL_HANDLE};

  GFXTexture2D m_Texture;
};

class SceneTexturedTriangleFactory : public SceneFactory {
private:
  const std::string &getName() const override;
  std::unique_ptr<Scene> instantiate() override;
};