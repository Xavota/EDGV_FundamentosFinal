#pragma once

#include "tools/texture.h"

//class Material {
// public:
//  /**
//   * @brief  Default constructor.
//   */
//  Material() = default;
//  /**
//   * @brief  Default destructor.
//   */
//  ~Material() = default;
//
//  /**
//   * @brief  Setter of the texture pointer of the material.
//   * @param    tex  The new texture pointer. Is a contant value because the
//   *                Material won't change anything in the texture,
//   *                it just stores it.
//   */
//  void setTexture(const sf::Texture* tex) {
//    m_pTexture = tex;
//  }
//  /**
//   * @brief  Getter of the texture pointer of the material.
//   */
//  const sf::Texture* getTexture() const {
//    return m_pTexture;
//  }
//
//  /**
//   * @brief  Setter of the color of the material.
//   * @param    color  The new color.
//   */
//  void setColor(const sf::Color& color) {
//    m_color = color;
//  }
//  /**
//   * @brief  Getter of the color of the material.
//   */
//  sf::Color getColor() const {
//    return m_color;
//  }
//
// private:
//  /**
//   * @brief  The main texture of the material. This can be null, indicating the
//   *         material is a plain color.
//   */
//  const sf::Texture* m_pTexture;
//  /**
//   * @brief  The color tint of the material. If the texture is null, the
//   *         material would be only this color, if the texture is not true,
//   *         the color will tint the texture.
//   */
//  sf::Color m_color;
//};

struct Material
{
  /**
   * @brief  The main texture of the material. This can be null, indicating the
   *         material is a plain color.
   */
  sf::Texture* m_pTexture = nullptr;
  /**
   * @brief  The color tint of the material. If the texture is null, the
   *         material would be only this color, if the texture is not true,
   *         the color will tint the texture.
   */
  sf::Color m_color = sf::Color::White;
};