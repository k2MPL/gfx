#pragma once

#include <Scene.h>

#include <gfx/GFXShader.h>
#include <gfx/GFXTexture2D.h>

#include <glm/matrix.hpp>

class SceneTexturedCube : public Scene {
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

  struct Mesh;
  bool loadMesh(const std::string &_path, Mesh &_mesh);
  void renderMesh(const Mesh &_mesh, const glm::mat4 &_worldMat,
                  const GFXTexture2D &_texture);

  bool loadTexture(const std::string &_path, GFXTexture2D &_texture);

private:
  glm::mat4 m_VPMatrix;

  GFXShaderProgram m_Program;

  struct Mesh {
    GLuint vao{INVALID_GL_HANDLE};
    GLuint vbo{INVALID_GL_HANDLE};
    GLuint ebo{INVALID_GL_HANDLE};

    GLsizei idxCount{0};
  };

  Mesh m_Cube;
  Mesh m_Plane;

  GFXTexture2D m_MossTexture;
  GFXTexture2D m_BrickTexture;
};

class SceneTexturedCubeFactory : public SceneFactory {
private:
  const std::string &getName() const override;
  std::unique_ptr<Scene> instantiate() override;
};