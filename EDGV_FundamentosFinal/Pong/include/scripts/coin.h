#pragma once

#include <scene/components/script.h>

class Coin : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  Coin() = default;
  /**
   * @brief Default destructor.
   */
  ~Coin() = default;

 private:
  void start() override;

  void onCollisionEnter(CollisionInfo info) override;
};