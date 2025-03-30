#pragma once

#include <scene/components/script.h>

class GameManager : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  GameManager() = default;
  /**
   * @brief Default destructor.
   */
  ~GameManager() = default;

 private:
  void start() override;
};