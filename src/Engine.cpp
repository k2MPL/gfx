#include <Engine.h>

#include <algorithm>

Engine Engine::gInstance;

Engine &Engine::Instance() { return gInstance; }

bool Engine::init(InitInfo &&_info) {
  m_SceneFactory = std::move(_info.sceneFactory);
  return switchScene(_info.initialScene);
}

void Engine::update() {}

void Engine::shutdown() {}

bool Engine::switchScene(const std::string &_name) {
  if (m_ActiveScene && (m_ActiveScene->getName() == _name))
    //#TODO: Log
    return true;

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
