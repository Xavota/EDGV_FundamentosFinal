#pragma once

#include "scene/component.h"

#include <SFML/Graphics.hpp>

class UIText : public Component
{
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kUIText };

  /**
   * @brief Default constructor.
   */
  UIText() = default;
  /**
   * @brief Default destructor.
   */
  ~UIText() = default;

  /**
   * @brief   Returns the component type.
   *
   * @return  The component type.
   */
  U8 getType() const override
  {
    return CmpType;
  }

  String m_sText = "";
  sf::Font* m_pFont = nullptr;
  U32 m_iFontSize = 30u;
  sf::Color m_color = sf::Color::White;
};