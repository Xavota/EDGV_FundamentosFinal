#include "scene/component.h"

#include "scene/actor.h"
#include "scene/components/transform.h"

WPtr<Transform> Component::getTransform() const
{
  return m_pActor.lock()->getTransform();
}

WPtr<Scene> Component::getScene() const
{
  return m_pActor.lock()->getScene();
}