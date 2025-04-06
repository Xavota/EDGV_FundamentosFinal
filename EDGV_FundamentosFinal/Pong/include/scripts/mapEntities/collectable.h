#pragma once

#include <scene/components/script.h>

class Player;

class Collectable : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  Collectable() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~Collectable() = default;

  FuntionPtr<void> m_fpCollected;

 protected:
  virtual void onCollected(WPtr<Player> player) {}

 private:
  void start() override;

  void onCollisionEnter(CollisionInfo info) override;
};