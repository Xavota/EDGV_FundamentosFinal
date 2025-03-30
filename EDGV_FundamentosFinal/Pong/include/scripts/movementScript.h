#pragma once

#include "scene/components/script.h"

class MovementScript : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  MovementScript() = default;
  /**
   * @brief Default destructor.
   */
  ~MovementScript() = default;

 private:
  void update() override;

  void onCollisionEnter(CollisionInfo info) const override;
  void onCollisionExit(CollisionInfo info) const override;

 float m_fSpeed = 5.0f;
};