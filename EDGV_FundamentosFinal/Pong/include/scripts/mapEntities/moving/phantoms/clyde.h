#pragma once

#include <scripts/mapEntities/moving/phantom.h>

class Clyde : public Phantom
{
 public:
  /**
   * @brief Default constructor.
   */
  Clyde() = default;
  /**
   * @brief Default destructor.
   */
  ~Clyde() = default;

  void init(WPtr<GameMap> map, float speed, U8 phantomIndex) override;

 private:
  String getAnimationName() const override { return "phantoms_clyde"; }
};