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

  void update() override;

  FunctionPtr<void> m_fpOnClick;
  FunctionPtr<void> m_fpOnClickContinue;
  FunctionPtr<void> m_fpOnClickStops;
  FunctionPtr<void> m_fpOnMouseEnter;
  FunctionPtr<void> m_fpOnHovered;
  FunctionPtr<void> m_fpOnMouseExit;

 private:
  bool m_bMouseOver = false;
  bool m_bBeingClicked = false;
};