#pragma once

#include "scene/component.h"

#include "resources/collider.h"

class Actor;

class Script : public Component {
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kScript };

  /**
   * @brief Default constructor.
   */
  Script() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~Script() = default;

  /**
   * @brief   Returns the component type.
   * @return  The component type.
   * @bug    No know Bugs
   */
  virtual U8 getType() const override
  {
    return CmpType;
  }

 protected:
  friend class Actor;

  /**
   * @brief  The start of the life of this script in the scene.
   * @bug    No know Bugs
   */
  virtual void start() {}

  /**
   * @brief  If this script is on an actor that has a collider component active
   *         and it hits or gets hit (no real difference for this case) by
   *         another collider, call this function to notify the script of this
   *         hit event with the information of the collision point, normal and
   *         a reference to the other actor involved in this collision.
   * @param    info  The hit information. This has the point in space where the
   *                 collision occured, the normal vector of the other actor's
   *                 collider surface and a reference to the other actor
   *                 involved in the collision.
   * @bug    No know Bugs
   */
  virtual void onHit(HitInfo info) const {}

  /**
   * @brief  If this script is on an actor that has a collider component active
   *         and it starts colliding with another collider, call this function
   *         to notify the script that a collision started with a reference to
   *         the other actor involved in this collision.
   * @param    info  The collision information. This has a reference to the
   *                 other actor involved in the collision.
   * @bug    No know Bugs
   */
  virtual void onCollisionEnter(CollisionInfo info) const {}
  /**
   * @brief  If this script is on an actor that has a collider component active
   *         and it is colliding with another collider, call this function each
   *         frame the collision is happening to notify the script that is
   *         colliding, with a reference to the other actor involved in this
   *         collision.
   * @param    info  The collision information. This has a reference to the
   *                 other actor involved in the collision.
   * @bug    No know Bugs
   */
  virtual void onCollisionStay(CollisionInfo info) const {}
  /**
   * @brief  If this script is on an actor that has a collider component active
   *         and it was colliding with another collider las frame but now it
   *         isn't, call this function to notify the script that the collision
   *         ended, with a reference to the other actor involved in this
   *         collision.
   * @param    info  The collision information. This has a reference to the
   *                 other actor involved in the collision.
   * @bug    No know Bugs
   */
  virtual void onCollisionExit(CollisionInfo info) const {}
};