#pragma once

#include <scene/components/script.h>

class MapBuilder : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  MapBuilder() = default;
  /**
   * @brief Default destructor.
   */
  ~MapBuilder() = default;

 private:
  void init(const WString& mapFile);
};