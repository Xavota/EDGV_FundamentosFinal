#include "scene/actor.h"

#include "platform/stdHeaders.h"

#include "platform/memoryManager.hpp"

#include "scene/components/transform.h"
#include "scene/components/script.h"

void Actor::setActive(const bool active)
{
  m_bActive = active;

  if (m_bActive && !m_bIsStarted) {
    start();
  }
}

void Actor::init(const String& name, int sceneHashIndex)
{
  m_bIsStarted = false;

  m_sName = name;
  m_iSceneHashIndex = sceneHashIndex;
  if (getTransform().expired()) {
    SIZE cmpIndex = m_vComponents.size();
    m_vComponents.push_back(MemoryManager::createShared<Transform>());
    m_vComponents[cmpIndex]->init(shared_from_this());
  }
}

void Actor::start() {
  m_bIsStarted = true;

  for (auto& cmp : m_vComponents) {
    if (cmp->getType() == eCOMPONENT_TYPE::kScript && cmp->getActive())
      MemoryManager::sharedReinterpretCast<Script>(cmp)->start();
  }
}

void Actor::update()
{
  if (!m_bIsStarted) return;

  for (auto& cmp : m_vComponents) {
    if (cmp->getActive()){
      cmp->update();
    }
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
  m_bIsStarted = false;
}

void Actor::onHit(HitInfo info) const {
  for (auto& cmp : m_vComponents) {
    if (cmp->getType() == eCOMPONENT_TYPE::kScript && cmp->getActive())
      MemoryManager::sharedReinterpretCast<Script>(cmp)->onHit(info);
  }
}

void Actor::onCollisionEnter(CollisionInfo info) const {
  for (auto& cmp : m_vComponents) {
    if (cmp->getType() == eCOMPONENT_TYPE::kScript && cmp->getActive())
      MemoryManager::sharedReinterpretCast<Script>(cmp)->onCollisionEnter(info);
  }
}

void Actor::onCollisionStay(CollisionInfo info) const {
  for (auto& cmp : m_vComponents) {
    if (cmp->getType() == eCOMPONENT_TYPE::kScript && cmp->getActive())
      MemoryManager::sharedReinterpretCast<Script>(cmp)->onCollisionStay(info);
  }
}

void Actor::onCollisionExit(CollisionInfo info) const {
  for (auto& cmp : m_vComponents) {
    if (cmp->getType() == eCOMPONENT_TYPE::kScript && cmp->getActive())
      MemoryManager::sharedReinterpretCast<Script>(cmp)->onCollisionExit(info);
  }
}