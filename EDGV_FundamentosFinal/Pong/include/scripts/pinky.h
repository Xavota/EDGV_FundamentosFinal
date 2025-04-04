#pragma once

#include <scripts/phantom.h>

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

 private:
  String getAnimationName() const override { return "phantoms_pinky"; }
};