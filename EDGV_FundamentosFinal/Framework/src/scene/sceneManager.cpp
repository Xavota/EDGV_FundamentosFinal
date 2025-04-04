#include "scene/sceneManager.h"

#include "scene/scene.h"

#include "platform/memoryManager.hpp"

WPtr<Scene> SceneManager::addScene(const String& name)
{
  if (m_mScenes.find(name) != m_mScenes.end()) {
    return getScene(name);
  }

  m_mScenes[name] = MemoryManager::createShared<Scene>();
  m_mScenes[name]->init();
  m_mScenes[name]->m_bRenderDebugGrid = m_bDebug;

  return m_mScenes[name];
}

WPtr<Scene> SceneManager::getScene(const String& name) const
{
  auto sceneIt = m_mScenes.find(name);
  if (sceneIt != m_mScenes.end()) {
    return sceneIt->second;
  }

  return {};
}

void SceneManager::deleteScene(const String& name)
{
  auto sceneIt = m_mScenes.find(name);
  if (sceneIt != m_mScenes.end()) {
    sceneIt->second->destroy();
    m_mScenes.erase(sceneIt);
  }
}

void SceneManager::openScene(const String& name)
{
  auto sceneIt = m_mScenes.find(name);
  if (sceneIt == m_mScenes.end()) return;


  for (auto& sc : m_mScenes) {
    if (sc.first == name) {
      sceneIt->second->m_bIsActive = true;
      sceneIt->second->start();
      continue;
    }

    sceneIt->second->m_bIsActive = false;
    sceneIt->second->unstart();
  }
}


void SceneManager::init() {
  m_mScenes = {};
}

void SceneManager::update()
{
  for (auto& scene : m_mScenes) {
    if (!scene.second->m_bIsActive) continue;

    scene.second->update();
  }
}

void SceneManager::render(sf::RenderWindow* window) const
{
  if (nullptr == window) return;

  for (auto& scene : m_mScenes) {
    if (!scene.second->m_bIsActive) continue;

    scene.second->render(window);
  }
  for (auto& scene : m_mScenes) {
    if (!scene.second->m_bIsActive) continue;

    scene.second->renderUI(window);
  }
}

void SceneManager::destroy()
{
  for (auto& scene : m_mScenes) {
    scene.second->destroy();
  }
  m_mScenes.clear();
}