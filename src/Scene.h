#pragma once

#include <memory>
#include <string>

class Scene {
public:
  virtual ~Scene() = default;

  void update(float _delta);
  void shutdown();

  virtual const std::string &getName() const = 0;

protected:
  virtual void onUpdate(float _delta) {}
  virtual void onShutdown() {}
};

class SceneFactory {
public:
  virtual ~SceneFactory() = default;

  virtual const std::string &getName() const = 0;
  virtual std::unique_ptr<Scene> &&instantiate() = 0;
};