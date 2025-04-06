#pragma once

#include <scripts/mapEntities/moving/phantom.h>

class Inky : public Phantom
{
 public:
  /**
   * @brief Default constructor.
   */
  Inky() = default;
  /**
   * @brief Default destructor.
   */
  ~Inky() = default;

  void init(WPtr<GameMap> map, float speed, U8 phantomIndex) override;
  void reset() override;

 private:
  String getAnimationName() const override { return "phantoms_inky"; }
};