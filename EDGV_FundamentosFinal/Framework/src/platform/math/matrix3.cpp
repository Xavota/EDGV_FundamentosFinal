#include "platform/math/matrix3.h"

#include "platform/stdHeaders.h"

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
  Matrix3 inv = {};
  
  // Compute the determinant
  double det = m_00 * (m_11 * m_22 - m_12 * m_21) -
               m_01 * (m_10 * m_22 - m_12 * m_20) +
               m_02 * (m_10 * m_21 - m_11 * m_20);

  if (det == 0.0) {
    std::cout << "Wrong matrix for inverse. Determinant = 0." << std::endl;
    return {};
  }

  double invDet = 1.0 / det;

  // Compute the inverse
  inv.m_00 =  (m_11 * m_22 - m_12 * m_21) * invDet;
  inv.m_01 = -(m_01 * m_22 - m_02 * m_21) * invDet;
  inv.m_02 =  (m_01 * m_12 - m_02 * m_11) * invDet;

  inv.m_10 = -(m_10 * m_22 - m_12 * m_20) * invDet;
  inv.m_11 =  (m_00 * m_22 - m_02 * m_20) * invDet;
  inv.m_12 = -(m_00 * m_12 - m_02 * m_10) * invDet;

  inv.m_20 =  (m_10 * m_21 - m_11 * m_20) * invDet;
  inv.m_21 = -(m_00 * m_21 - m_01 * m_20) * invDet;
  inv.m_22 =  (m_00 * m_11 - m_01 * m_10) * invDet;

  return inv;
}