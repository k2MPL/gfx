#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <Engine.h>

#include <Scene_TexturedCube.h>
#include <Scene_TexturedTriangle.h>
#include <Scene_Triangle.h>

static void *LoadGLFunc(const char *name) { return glfwGetProcAddress(name); }

int main() {
  GLFWwindow *window;
  if (!glfwInit())
    return 0;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(1024, 768, "GFX", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return 0;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  gladLoadGLLoader(LoadGLFunc);

  Engine::InitInfo engineInitInfo;
  engineInitInfo.initialScene = SceneTexturedTriangle::gSceneName;

  engineInitInfo.sceneFactory.push_back(
      std::unique_ptr<SceneFactory>{new SceneTriangleFactory{}});

  engineInitInfo.sceneFactory.push_back(
      std::unique_ptr<SceneFactory>{new SceneTexturedCubeFactory{}});

  engineInitInfo.sceneFactory.push_back(
      std::unique_ptr<SceneFactory>(new SceneTexturedTriangleFactory{}));

  if (!ENGINE.init(std::move(engineInitInfo))) {
    return 0;
  }

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    ENGINE.update();
    glfwSwapBuffers(window);
  }

  ENGINE.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}