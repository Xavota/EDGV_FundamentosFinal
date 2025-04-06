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

  FuntionPtr<void> m_fpOnClick;
  FuntionPtr<void> m_fpOnClickContinue;
  FuntionPtr<void> m_fpOnClickStops;
  FuntionPtr<void> m_fpOnMouseEnter;
  FuntionPtr<void> m_fpOnHovered;
  FuntionPtr<void> m_fpOnMouseExit;

 private:
  bool m_bMouseOver = false;
  bool m_bBeingClicked = false;
};