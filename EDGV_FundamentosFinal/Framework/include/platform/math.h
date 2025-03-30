#pragma once
#include <SFML/Graphics.hpp>

#include "platform/defines.h"

class Math
{
 public:
  /**
   * @brief      Returns the length of the vector
   * @param      vec: the vector checking
   * @bug      No know Bugs
   */
  static float vectorLength(sf::Vector2f vec);
  /**
   * @brief      Returns the vector normalized
   * @param      vec: the vector checking
   * @bug      No know Bugs
   */
  static sf::Vector2f normalizeVector(sf::Vector2f vec);
  /**
   * @brief      Returns the vector with the length specified
   * @param      vec: the vector checking
   * @param      length: the new length of the vector
   * @bug      No know Bugs
   */
  static sf::Vector2f truncateVector(sf::Vector2f vec, float length);
  /**
   * @brief      Returns the distance between two vectors
   * @param      vec1: the first vector checking
   * @param      vec2: the second vector checking
   * @bug      No know Bugs
   */
  static float distanceVector(sf::Vector2f vec1, sf::Vector2f vec2);
  /**
   * @brief      Returns the dot product between 2 vectors
   * @param      vec1: the first vector checking
   * @param      vec2: the second vector checking
   * @bug      No know Bugs
   */
  static float dot(sf::Vector2f vec1, sf::Vector2f vec2);
  /**
   * @brief      Returns the right vector of the vector passed
   * @param      vec: the vector checking
   * @bug      No know Bugs
   */
  static sf::Vector2f rightVector(sf::Vector2f vec);

  /**
   * @brief      Returns the right vector of the vector passed
   * @param      line: the rectangle shape the will be turning into the line needed
   * @param      pos1: start position
   * @param      pos2: end position
   * @bug      No know Bugs
   */
  static void placeLineFromTwoPoints(sf::RectangleShape& line, sf::Vector2f pos1, sf::Vector2f pos2);


  /**
   * @brief  Check if the value has the 1's bits of the flag.
   * @param    _val   The value to check.
   * @param    _flag  The flag to check the 1's bits.
   * @return If the value has the 1's bits of the flag.
   */
  template<typename V, typename F>
  static FORCEINLINE bool hasFlag(V _val, F _flag);


  /**
   * @brief  The value of PI
   */
  static float kPI;
  /**
   * @brief  The value of PI times 2
   */
  static float k2PI;
  /**
   * @brief  The value of PI divided by 2
   */
  static float kPIOver2;
};

template<typename V, typename F>
FORCEINLINE bool Math::hasFlag(V _val, F _flag)
{
  return (_val & _flag) == _flag;
}