#pragma once

#include <SFML/Graphics.hpp>

#include "platform/stdHeaders.h"

class Actor;

/**
 * @brief The hit descriptor. Describes where a collision between two
 *        objects happen, as well as the normal vector of the surface of the
 *        colliding object and a reference to the actor that owns the collision
 *        component. This object is different for the two actors involved in the
 *        collision, because the normal vector and the reference would be of the
 *        other actor, that's not them.
 */
struct HitInfo {
  /**
   * @brief The point in space where the collision happend. This is calculated
   *        post collision, and normally isn't exact, because of the framerate.
   *        The collision is detected when in a previous frame the two object
   *        where not touching and in this frame they are, but at this point the
   *        two objects might be touching in multiple points, so the collision
   *        detection tries to predict where collision might have started.
   */
  sf::Vector2f point;
  /**
   * @brief The normal vector of the surface where the collision point is on.
   *        This is different for both actors involved in the collision, since
   *        both of them are told the normal vector of the other actor, and not
   *        their own.
   */
  sf::Vector2f normal;
  /**
   * @brief The other actor involved in the collision. This is a reference to
   *        the opposite actor in the collision, so it's different for both
   *        actors.
   */
  WPtr<Actor> otherActor;
};

/**
 * @brief The collision descriptor. Contains the other actor involved in the
 *        collision.
 */
struct CollisionInfo {
  /**
   * @brief The other actor involved in the collision. This is a reference to
   *        the opposite actor in the collision, so it's different for both
   *        actors.
   */
  WPtr<Actor> otherActor;
};