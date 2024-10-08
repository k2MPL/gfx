#pragma once

#include <memory>
#include <vector>

#include <Scene.h>

#include <interface_FileSystem.h>
#include <interface_Logger.h>

class Engine {
private:
  static Engine gInstance;

  Engine(const Engine &) = delete;
  Engine(Engine &&) = delete;

  Engine &operator=(const Engine &) = delete;
  Engine &operator=(Engine &&) = delete;

public:
  static Engine &Instance();

public:
  Engine() = default;

  struct InitInfo {
    std::string initialScene;
    std::vector<std::unique_ptr<SceneFactory>> sceneFactory;

    InitInfo() = default;
    InitInfo(InitInfo &&_movable) {
      initialScene = std::move(_movable.initialScene);
      sceneFactory = std::move(_movable.sceneFactory);
    }
  };

public:
  bool init(InitInfo &&_info);
  void update();
  void shutdown();

  Logger &getLogger();
  FileSystem &getFileSystem();

private:
  bool switchScene(const std::string &_name);

private:
  Logger m_Logger;
  FileSystem m_FileSystem;

  std::unique_ptr<Scene> m_ActiveScene;
  std::vector<std::unique_ptr<SceneFactory>> m_SceneFactory;
};

#define ENGINE Engine::Instance()
#define LOGGER Engine::Instance().getLogger()
#define FILE_SYSTEM Engine::Instance().getFileSystem()