#pragma once

#include <SFML/Graphics.hpp>

#include "platform/defines.h"

class Matrix3
{
 public:
  /**
   * @brief  Default constructor
   */
  Matrix3() = default;
  /**
   * @brief  Default destructor
   */
  ~Matrix3() = default;

  /**
   * @brief  Construct the matrix value by value.
   */
  Matrix3(const float m_00, const float m_01, const float m_02,
          const float m_10, const float m_11, const float m_12,
          const float m_20, const float m_21, const float m_22) :
          m_00(m_00), m_01(m_01), m_02(m_02),
          m_10(m_10), m_11(m_11), m_12(m_12),
          m_20(m_20), m_21(m_21), m_22(m_22) {}
  /**
   * @brief  Construct the matrix by X and Y axis and a translation, for
   *         transformation matrices.
   */
  Matrix3(const sf::Vector2f& xAxis,
          const sf::Vector2f& yAxis,
          const sf::Vector2f& translation) :
          m_00(xAxis.x), m_01(yAxis.x), m_02(translation.x),
          m_10(xAxis.y), m_11(yAxis.y), m_12(translation.y),
          m_20(0.0f),    m_21(0.0f),    m_22(1.0f) {}
  /**
   * @brief  Copy constructor.
   */
  Matrix3(const Matrix3& other) :
  m_00(other.m_00), m_01(other.m_01), m_02(other.m_02),
  m_10(other.m_10), m_11(other.m_11), m_12(other.m_12),
  m_20(other.m_20), m_21(other.m_21), m_22(other.m_22) {}

  /**
   * @brief  Get the X axis transformation of the transform matrix.
   *         This is the first 2 values of the first column. (m_00 and m_10)
   *         x o o
   *         x o o
   *         o o o
   */
  FORCEINLINE sf::Vector2f getXAxis() const { return { m_00, m_10 }; }
  /**
   * @brief  Get the Y axis transformation of the transform matrix.
   *         This is the first 2 values of the second column. (m_01 and m_11)
   *         o x o
   *         o x o
   *         o o o
   */
  sf::Vector2f getYAxis() const { return { m_01, m_11 }; }
  /**
   * @brief  Get the translation of the transform matrix.
   *         This is the first 2 values of the third column. (m_02 and m_12)
   *         o o x
   *         o o x
   *         o o o
   */
  sf::Vector2f getTranslation() const { return { m_02, m_12 }; }

  /**
   * @brief  Transform the position of the given point using the transform
   *         matrix.
   * @param    point  The point to be transformed.
   */
  sf::Vector2f transform(const sf::Vector2f& point) const;

  /**
   * @brief  Multiply this matrix times the [other] matrix. (this * other)
   * @param    other  The other matrix to multiply with.
   */
  Matrix3 mul(const Matrix3& other) const;

  /**
   * @brief  Return the inverse of this matrix.
   */
  Matrix3 getInverse() const;

  /**
   * @brief  Default constructor
   */
  float m_00, m_01, m_02;
  float m_10, m_11, m_12;
  float m_20, m_21, m_22;
};