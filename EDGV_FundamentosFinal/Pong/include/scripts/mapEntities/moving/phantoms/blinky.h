#pragma once

#include <scripts/mapEntities/moving/phantom.h>

class Blinky : public Phantom
{
 public:
  /**
   * @brief Default constructor.
   */
  Blinky() = default;
  /**
   * @brief Default destructor.
   */
  ~Blinky() = default;

  void init(WPtr<GameMap> map, float speed, U8 phantomIndex) override;
  void reset() override;

 private:
  String getAnimationName() const override { return "phantoms_blinky"; }
};