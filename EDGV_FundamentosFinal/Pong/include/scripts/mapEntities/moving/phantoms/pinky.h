#pragma once

#include <scripts/mapEntities/moving/phantom.h>

class Pinky : public Phantom
{
 public:
  /**
   * @brief Default constructor.
   */
  Pinky() = default;
  /**
   * @brief Default destructor.
   */
  ~Pinky() = default;

  void init(WPtr<GameMap> map, float speed, U8 phantomIndex) override;

 private:
  String getAnimationName() const override { return "phantoms_pinky"; }
};