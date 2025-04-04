#pragma once

#include "platform/defines.h"
#include "platform/stdHeaders.h"

class Scene;
class Actor;
class Transform;

/**
 * @brief Types of components.
 */
namespace eCOMPONENT_TYPE {
  enum E : U8
  {
    kNone = 0,
    kTransform,
    kRender,
    kShapeRect,
    kShapeCircle,
    kCollision,
    kCircleCollision,
    kRectCollision,
    kScript,
  };
}

class Component
{
 public:
  /**
   * @brief The enum for identifying the component type. An alternative to rtti.
   */
  enum { CmpType = eCOMPONENT_TYPE::kNone };

  /**
   * @brief  Default constructor
   */
  Component() = default;
  /**
   * @brief  Default destructor
   */
  virtual ~Component() = default;

  /**
   * @brief   Returns the component type.
   * @return  The component type.
   */
  FORCEINLINE virtual U8 getType() const
  {
    return CmpType;
  }

  /**
   * @brief  Updates the everything the component needs.
   */
  virtual void update() {}

  /**
   * @brief   Returns true if the component is active and can update.
   * @return  If the component is active.
   */
  FORCEINLINE virtual bool getActive() const
  {
    return m_bActive;
  }
  /**
   * @brief  Set if the actor can update and do stuff.
   * @param    active If the actor can update and do stuff.
   */
  FORCEINLINE virtual void setActive(const bool active)
  {
    m_bActive = active;
  }

  /**
   * @brief   Returns the actor to witch the component is attached.
   * @return  The actor to witch the component is attached.
   */
  FORCEINLINE virtual WPtr<Actor> getActor() const
  {
    return m_pActor;
  }

  /**
   * @brief   A shortcut for getting the transform component of the actor that
   *          owns this component.
   * @return  The transform component of the actor that owns this component.
   */
  WPtr<Transform> getTransform() const;

  /**
   * @brief   A short cut for getting the scene the actor that owns this
   *          component is on.
   * @return  The scene this actor is on.
   * @bug     No know Bugs
   */
  WPtr<Scene> getScene() const;

 protected:
  /**
   * @brief The actor to witch it's attached.
   */
  WPtr<Actor> m_pActor;
 
  /**
   * @brief If the actor can update and do stuff.
   */
  bool m_bActive = true;

 private:
  friend class Actor;

  /**
   * @brief  Initializes all the things in the component and tells it the actor attached
   *         to.
   * @param    act  The actor attached to.
   */
  void init(const WPtr<Actor> act)
  {
    m_pActor = act;
  }
};