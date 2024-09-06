#include <Scene.h>

void Scene::update(float _delta) { onUpdate(_delta); }

void Scene::shutdown() { onShutdown(); }