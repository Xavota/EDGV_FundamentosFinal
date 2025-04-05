#pragma once

#include "scene/component.h"

class Canvas : public Component
{
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kCanvas };

  /**
   * @brief Default constructor.
   */
  Canvas() = default;
  /**
   * @brief Default destructor.
   */
  ~Canvas() = default;

  /**
   * @brief   Returns the component type.
   *
   * @return  The component type.
   */
  U8 getType() const override
  {
    return CmpType;
  }
};