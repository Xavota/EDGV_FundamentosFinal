#pragma once

#include "scene/components/collider.h"

#include <SFML/Graphics.hpp>

class CircleCollider : public Collider {
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kCircleCollision };

  /**
   * @brief Default constructor.
   */
  CircleCollider() = default;
  /**
   * @brief Default destructor.
   */
  ~CircleCollider() = default;

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
  float scaleRadius = 1.0f;

 private:
  friend class Scene;

  /**
   * @brief The precalculated real radius. This is only modified by the Scene
   *        and is used to calculate its collisions. The real radius is equal
   *        to the max global scale axis times the [scaleRadius]
   */
  float m_fRealRadius;
};