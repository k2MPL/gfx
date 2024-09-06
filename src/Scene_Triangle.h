#pragma once

#include <Scene.h>

class SceneTriangle : public Scene {
public:
  static constexpr const char *gSceneName = "Triangle";
  const std::string &getName() const override;
};

class SceneTriangleFactory : public SceneFactory {
private:
  const std::string &getName() const override;
  std::unique_ptr<Scene> &&instantiate() override;
};