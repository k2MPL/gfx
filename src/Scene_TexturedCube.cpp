#include <Scene_TexturedCube.h>

const std::string &SceneTexturedCube::getName() const { return gSceneName; }

const std::string &SceneTexturedCubeFactory::getName() const {
  return SceneTexturedCube::gSceneName;
}

std::unique_ptr<Scene> &&SceneTexturedCubeFactory::instantiate() {
  return std::unique_ptr<Scene>{new SceneTexturedCube{}};
}