#include "tools/math.h"

#include <iostream>

namespace gl
{
  float Math::kPI = 3.14159264f;
  float Math::k2PI = kPI * 2.0f;
  float Math::kPIOver2 = kPI * 0.5f;

  float Math::vectorLength(sf::Vector2f vec)
  {
    return pow(vec.x * vec.x + vec.y * vec.y, .5f);
  }
  
  sf::Vector2f Math::normalizeVector(sf::Vector2f vec)
  {
    float len = vectorLength(vec);
    if (len == 0)
    {
      return sf::Vector2f(0, 0);
    }
    return vec / len;
  }
  
  sf::Vector2f Math::truncateVector(sf::Vector2f vec, float length)
  {
    return normalizeVector(vec) * length;
  }
  
  float Math::distanceVector(sf::Vector2f vec1, sf::Vector2f vec2)
  {
    return vectorLength(vec2 - vec1);
  }
  
  float Math::dot(sf::Vector2f vec1, sf::Vector2f vec2)
  {
    return vec1.x * vec2.x + vec1.y * vec2.y;
  }
  
  sf::Vector2f Math::rightVector(sf::Vector2f vec)
  {
    return sf::Vector2f(-vec.y, vec.x);
  }
  
  void Math::placeLineFromTwoPoints(sf::RectangleShape& line, sf::Vector2f pos1, sf::Vector2f pos2)
  {
    line.setPosition(pos1);  
    sf::Vector2f ori = pos2 - pos1;
    line.setSize({ 3.f, vectorLength(ori) });

    /*float angle = atan((ori.y) / (ori.x)); // in radians
    if (ori.x < 0)
    {
      angle += kPI; //
    }
    else if (ori.y < 0)
    {
      angle += k2PI;
    }*/

    float angle = std::atan2(ori.y, ori.x); // in radians

    //line.setRotation(sf::Angle(angle - 90));
    line.setRotation(sf::radians(angle - kPIOver2));
  }
}