#pragma once

#include <Scene.h>

class SceneTexturedCube : public Scene {
public:
  static constexpr const char *gSceneName = "Textured Cube";
  const std::string &getName() const override;
};

class SceneTexturedCubeFactory : public SceneFactory {
private:
  const std::string &getName() const override;
  std::unique_ptr<Scene> &&instantiate() override;
};