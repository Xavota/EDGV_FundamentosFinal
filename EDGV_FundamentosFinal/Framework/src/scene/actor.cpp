#include "scene/actor.h"

#include "platform/stdHeaders.h"

#include "platform/memoryManager.hpp"

#include "scene/components/transform.h"

void Actor::init(const String& name, int sceneHasIndex)
{
  m_sName = name;
  m_iSceneHashIndex = sceneHasIndex;
  if (getTransform().expired()) {
    SIZE cmpIndex = m_vComponents.size();
    m_vComponents.push_back(MemoryManager::createShared<Transform>());
    m_vComponents[cmpIndex]->init(shared_from_this());
  }
}

void Actor::update()
{
  for (auto& cmp : m_vComponents) {
    if (cmp->getActive())
      cmp->update();
  }
}

bool Actor::isActive() const
{
  auto transform = getTransform().lock();
  auto parent = transform->getParent();
  if (parent.expired()) {
    return m_bActive;
  }

  return parent.lock()->getActor().lock()->isActive() && m_bActive;
}

void Actor::destroy()
{
  m_vComponents.clear();
}