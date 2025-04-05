#pragma once

#include "scene/component.h"

#include <SFML/Graphics.hpp>

class Button : public Component
{
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kButton };

  /**
   * @brief Default constructor.
   */
  Button() = default;
  /**
   * @brief Default destructor.
   */
  ~Button() = default;

  /**
   * @brief   Returns the component type.
   *
   * @return  The component type.
   */
  U8 getType() const override
  {
    return CmpType;
  }

  FuntionPtr<void> m_fpOnClick;
};