#include "scene/components/transform.h"

#include "scene/actor.h"

Transform::~Transform()
{
  if (m_pParent.expired()) return;

  auto& children = m_pParent.lock()->getTransform().lock()->m_vChildren;

  auto childIt = std::find_if(children.begin(), children.end(),
                              [this](WPtr<Transform> tr){
                                SPtr<Actor> child = tr.lock()->getActor().lock();
                                SPtr<Actor> actor = this->getActor().lock();
                                return child->getName() == actor->getName() &&
                                 child->m_iSceneHashIndex == actor->m_iSceneHashIndex;
                              });

  if (childIt == children.end()) return;

  children.erase(childIt);
}

Matrix3 Transform::getLocalModelMatrix() const
{
  sf::Vector2f xAxis = {
    std::cos(m_rotation.asRadians()) * m_scale.x,
    std::sin(m_rotation.asRadians()) * m_scale.y 
  };
  sf::Vector2f yAxis = {
    -std::sin(m_rotation.asRadians()) * m_scale.x,
     std::cos(m_rotation.asRadians()) * m_scale.y 
  };

  return Matrix3(xAxis, yAxis, m_position);
}

Matrix3 Transform::getModelMatrix() const
{
  if (m_pParent.expired()) {
    return getLocalModelMatrix();
  }

  return m_pParent.lock()->getModelMatrix().mul(getLocalModelMatrix());
}

sf::Vector2f Transform::getLocalPosition() const
{
  return m_position;
}

sf::Vector2f Transform::getPosition() const
{
  if (m_pParent.expired()) {
    return m_position;
  }

  return m_pParent.lock()->getModelMatrix().transform(m_position);
}

void Transform::setLocalPosition(const sf::Vector2f& pos)
{
  m_position = pos;
}

void Transform::setPosition(const sf::Vector2f& pos)
{
  if (m_pParent.expired()) {
    m_position = pos;
    return;
  }

  m_position = m_pParent.lock()->getModelMatrix().getInverse().transform(pos);
}

sf::Angle Transform::getLocalRotation() const
{
  return m_rotation;
}

sf::Angle Transform::getRotation() const
{
  if (m_pParent.expired()) {
    return m_rotation;
  }

  return m_pParent.lock()->getRotation() + m_rotation;
}

void Transform::setLocalRotation(const sf::Angle& rot)
{
  m_rotation = rot;
}

void Transform::setRotation(const sf::Angle& rot)
{
  if (m_pParent.expired()) {
    m_rotation = rot;
    return;
  }

  m_rotation = rot - m_pParent.lock()->getRotation();
}

sf::Vector2f Transform::getLocalScale() const
{
  return m_scale;
}

sf::Vector2f Transform::getScale() const
{
  if (m_pParent.expired()) {
    return m_scale;
  }

  auto parentScale = m_pParent.lock()->getScale();
  return {parentScale.x * m_scale.x, parentScale.y * m_scale.y};
}

void Transform::setLocalScale(const sf::Vector2f& scale)
{
  m_scale = scale;
}

void Transform::setScale(const sf::Vector2f& scale)
{
  if (m_pParent.expired()) {
    m_scale = scale;
    return;
  }

  auto parentScale = m_pParent.lock()->getScale();
  m_scale = {scale.x / parentScale.x, scale.y / parentScale.y};
}

void Transform::attachTo(const WPtr<Transform> transformParent,
                         const bool retransform)
{
  if (retransform) {
    auto gPos = getPosition();
    auto gRot = getRotation();
    auto gScale = getScale();

    m_pParent = transformParent;
    m_pParent.lock()->getTransform().lock()->m_vChildren.push_back(
      getActor().lock()->getTransform());

    setPosition(gPos);
    setRotation(gRot);
    setScale(gScale);

    return;
  }

  m_pParent = transformParent;
  m_pParent.lock()->getTransform().lock()->m_vChildren.push_back(
    getActor().lock()->getTransform());
}