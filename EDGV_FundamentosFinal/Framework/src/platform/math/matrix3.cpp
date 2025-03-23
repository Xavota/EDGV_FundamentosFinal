#include "platform/math/matrix3.h"

sf::Vector2f Matrix3::transform(const sf::Vector2f& point) const
{
  return {
    m_00 * point.x + m_01 * point.y + m_02,
    m_10 * point.x + m_11 * point.y + m_12
  };
}

Matrix3 Matrix3::mul(const Matrix3& other) const
{
  return Matrix3(
    this->m_00 * other.m_00 + this->m_01 * other.m_10 + this->m_02 + other.m_20,
    this->m_00 * other.m_01 + this->m_01 * other.m_11 + this->m_02 + other.m_21,
    this->m_00 * other.m_02 + this->m_01 * other.m_12 + this->m_02 + other.m_22,

    this->m_10 * other.m_00 + this->m_11 * other.m_10 + this->m_12 + other.m_20,
    this->m_10 * other.m_01 + this->m_11 * other.m_11 + this->m_12 + other.m_21,
    this->m_10 * other.m_02 + this->m_11 * other.m_12 + this->m_12 + other.m_22,

    this->m_20 * other.m_00 + this->m_21 * other.m_10 + this->m_22 + other.m_20,
    this->m_20 * other.m_01 + this->m_21 * other.m_11 + this->m_22 + other.m_21,
    this->m_20 * other.m_02 + this->m_21 * other.m_12 + this->m_22 + other.m_22
  );
}

/// TODO: IMPLEMENTAR ESTA FUNCIÓN.
Matrix3 Matrix3::getInverse() const
{
  return Matrix3();
}