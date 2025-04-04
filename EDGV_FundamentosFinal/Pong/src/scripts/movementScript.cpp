#include "scripts/movementScript.h"

#include <scene/actor.h>
#include <scene/components/transform.h>

#include <tools/input.h>
#include <tools/time.h>

void MovementScript::update()
{
  sf::Vector2f movement = {};
  if (gl::Input::getKey(sf::Keyboard::Key::W)) {
    movement += sf::Vector2f{0.0f, -1.0f};
  }
  if (gl::Input::getKey(sf::Keyboard::Key::S)) {
    movement += sf::Vector2f{0.0f, 1.0f};
  }
  if (gl::Input::getKey(sf::Keyboard::Key::A)) {
    movement += sf::Vector2f{-1.0f, 0.0f};
  }
  if (gl::Input::getKey(sf::Keyboard::Key::D)) {
    movement += sf::Vector2f{1.0f, 0.0f};
  }

  if (movement == sf::Vector2f{}) return;

  SPtr<Transform> transform = getTransform().lock();
  sf::Vector2f newPos = transform->getPosition() +
   movement.normalized() * m_fSpeed * gl::Time::instance().deltaTime();
  transform->setPosition(newPos);
}

void MovementScript::onCollisionEnter(CollisionInfo info)
{
  std::cout << "Enter collision with: " << info.otherActor.lock()->getName() << std::endl;
}

void MovementScript::onCollisionExit(CollisionInfo info)
{
  std::cout << "Exit collision with: " << info.otherActor.lock()->getName() << std::endl;
}