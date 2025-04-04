#pragma once

#include <scripts/phantom.h>

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

 private:
  String getAnimationName() const override { return "phantoms_inky"; }
};