#pragma once

#include <SFML/Graphics.hpp>

#include "platform/stdHeaders.h"

#include "scene/component.h"


class Collider : public Component {
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kCollision };

  /**
   * @brief Default constructor.
   */
  Collider() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~Collider() = default;

  /**
   * @brief   Returns the component type.
   *
   * @return  The component type.
   */
  virtual U8 getType() const override
  {
    return CmpType;
  }


  /**
   * @brief Return the list of all the actors that are colliding with this
   *        colider component.
   */
  FORCEINLINE const Vector<WPtr<Actor>> getCollidingActors()
  {
    return m_vCollidingActors;
  }

 private:
  friend class Scene;

  /**
   * @brief The last global position of the collision. This to later calculation
   *        of collisions points and normals.
   */
  sf::Vector2f m_lastGlobalPosition;
  /**
   * @brief The current global position of the collision. This to calculate
   *        bounds of the collision, for optimizacion.
   */
  sf::Vector2f m_currentGlobalPosition;

  /**
   * @brief All the actors that are colliding with this colider component.
   *        This is to have a reference to them to know when the collision
   *        started, stayed and ended, and also for the actors to use if needed.
   */
  Vector<WPtr<Actor>> m_vCollidingActors;

 public:
  /**
   * @brief An offset to the collision to displace the center of the collision,
   *        in case you want to make it only on a certain area.
   */
  sf::Vector2f offset;

  /**
   * @brief DEBUG ONLY, THIS SHOULD BE FALSE WHEN BUILDING THE FINAL PROJECT.
   *        This makes the collision render, so you can visually see it in game.
   */
  bool render = false;
};