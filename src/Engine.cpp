#include <Engine.h>

#include <algorithm>

Engine Engine::gInstance;

Engine &Engine::Instance() { return gInstance; }

bool Engine::init(InitInfo &&_info) {
  m_SceneFactory = std::move(_info.sceneFactory);
  return switchScene(_info.initialScene);
}

void Engine::update() {
  if (m_ActiveScene) {
    m_ActiveScene->update(0.0f);
  }
}

void Engine::shutdown() {}

Logger &Engine::getLogger() { return m_Logger; }

FileSystem &Engine::getFileSystem() { return m_FileSystem; }

bool Engine::switchScene(const std::string &_name) {
  if (m_ActiveScene && (m_ActiveScene->getName() == _name)) {
    return true;
  }

  auto it =
      std::find_if(m_SceneFactory.begin(), m_SceneFactory.end(),
                   [&_name](const std::unique_ptr<SceneFactory> &_factory) {
                     return _factory->getName() == _name;
                   });

  if (it == m_SceneFactory.end())
    //#TODO: Log
    return false;

  if (m_ActiveScene) {
    m_ActiveScene->shutdown();
    m_ActiveScene = nullptr;
  }

  m_ActiveScene = (*it)->instantiate();
  return m_ActiveScene != nullptr;
}
