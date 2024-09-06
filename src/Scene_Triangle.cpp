#include <Scene_Triangle.h>

const std::string &SceneTriangle::getName() const { return gSceneName; }

const std::string &SceneTriangleFactory::getName() const {
  return SceneTriangle::gSceneName;
}

std::unique_ptr<Scene> &&SceneTriangleFactory::instantiate() {
  return std::unique_ptr<Scene>{new SceneTriangle{}};
}