#pragma once

#include <scripts/phantom.h>

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

 private:
  String getAnimationName() const override { return "phantoms_blinky"; }
};