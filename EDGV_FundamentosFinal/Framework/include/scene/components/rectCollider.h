#pragma once

#include "scene/components/collider.h"

#include <SFML/Graphics.hpp>

class RectCollider : public Collider {
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kRectCollision };

  /**
   * @brief Default constructor.
   */
  RectCollider() = default;
  /**
   * @brief Default destructor.
   */
  ~RectCollider() = default;

  /**
   * @brief   Returns the component type.
   *
   * @return  The component type.
   */
  U8 getType() const override
  {
    return CmpType;
  }


 public:
  /**
   * @brief A scaler of the circle collision. This collision adapts to the global
   *        scale of the transform of the actor it's on, so this is an aditional
   *        scaler to make the collision bigger or smaller.
   */
  sf::Vector2f scale = {1.0f, 1.0f};

 private:
  friend class Scene;

  /**
   * @brief The total radius from center to the furthest point of the collision.
   *        This to calculate bounds of the collision, for optimizacion.
   */
  float m_boundingRadius;
  /**
   * @brief The global scale of the rect collider, for precalculations of the
   *        collisions.
   */
  sf::Vector2f m_globalScale;
};