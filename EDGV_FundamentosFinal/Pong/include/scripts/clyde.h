#pragma once

#include <scripts/phantom.h>

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

 private:
  String getAnimationName() const override { return "phantoms_clyde"; }
};