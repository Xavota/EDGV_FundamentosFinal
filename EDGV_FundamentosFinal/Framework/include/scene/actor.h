#pragma once

#include "scene/component.h"

#include "platform/defines.h"
#include "platform/stdHeaders.h"
#include "platform/memoryManager.hpp"

#include "resources/collider.h"

#include "scene/components/transform.h"

class Scene;

class Actor : public EnableSPtrFromThis<Actor> {
 public:
  /**
   * @brief  Default constructor
   */
  Actor() = default;
  /**
   * @brief  Default destructor
   */
  ~Actor() = default;

  /**
   * @brief  Adds a component of the class of the template.
   * @bug    No know Bugs
   */
  template <class T>
  FORCEINLINE WPtr<T> addComponent();

  /**
   * @brief   Gets a component of the class of the template if it exists.
   * @return  The pointer to the component, nullptr if it doesn't exists.
   * @bug     No know Bugs
   */
  template <class T>
  FORCEINLINE WPtr<T> getComponent() const;
  /**
   * @brief  Returns all the components that the actor has, including Transform.
   * @return  All the components that the actor has, including Transform.
   * @bug     No know Bugs
   */
  FORCEINLINE const Vector<SPtr<Component>>& getAllComponents() const
  {
    return m_vComponents;
  }

  /**
   * @brief   Returns true if the actor is active on the scene.
   * @return  Whether the actor is active or not.
   * @bug     No know Bugs
   */
  bool isActive() const;
  /**
   * @brief   Returns true if the actor is active.
   * @return  Whether the actor is active or not.
   * @bug     No know Bugs
   */
  FORCEINLINE bool isLocalActive() const { return m_bActive; }
  /**
   * @brief  Sets if the actor is active or not on the scene.
   * @param    active: The new state of the actor. True if it's active.
   * @bug    No know Bugs
   */
  void setActive(const bool active);

  /**
   * @brief   Returns the transform component of the actor. To easy access.
   * @return  The transform component.
   * @bug     No know Bugs
   */
  FORCEINLINE WPtr<Transform> getTransform() const
  {
    return getComponent<Transform>();
  }
  
  /**
   * @brief   Returns the descriptive actor name.
   * @return  The actor name.
   * @bug     No know Bugs
   */
  FORCEINLINE const String& getName() const { return m_sName; }

  /**
   * @brief   Getter for a reference to the scene this actor is on. Usually for
   *          using in a script to add more actors on the same scene.
   * @return  The scene this actor is on.
   * @bug     No know Bugs
   */
  FORCEINLINE WPtr<Scene> getScene() const { return m_pScene; }

 private:
  /**
   * @brief  Initializes the actor, giving a name.
   * @param   name           The name of the actor. This need NOT to be unqiue,
   *                         since the [sceneHashIndex] will be used to create a
   *                         unique name.
   * @param   sceneHasIndex  The has index the actor has on the scene
   * @bug    No know Bugs
   */
  void init(const String& name, int sceneHashIndex);

  /**
   * @brief  Start the lifetime of this actor.
   * @bug    No know Bugs
   */
  void start();

  /**
   * @brief  Updates the actor and all of it components.
   * @bug    No know Bugs
   */
  virtual void update();

  /**
   * @brief  Frees the memory allocated on the actor.
   * @bug    No know Bugs
   */
  void destroy();

  /**
   * @brief  If this actor has a collider component active and it hits or gets
   *         hit (no real difference for this case) by another collider, call
   *         this function to notify the actor of this hit event with the
   *         information of the collision point, normal and a reference to the
   *         other actor involved in this collision.
   * @param    info  The hit information. This has the point in space where the
   *                 collision occured, the normal vector of the other actor's
   *                 collider surface and a reference to the other actor
   *                 involved in the collision.
   * @bug    No know Bugs
   */
  void onHit(HitInfo info) const;

  /**
   * @brief  If this actor has a collider component active and it starts
   *         colliding with another collider, call this function to notify the
   *         actor that a collision started with a reference to the other actor
   *         involved in this collision.
   * @param    info  The collision information. This has a reference to the
   *           other actor involved in the collision.
   * @bug    No know Bugs
   */
  void onCollisionEnter(CollisionInfo info) const;
  /**
   * @brief  If this actor has a collider component active and it is colliding
   *         with another collider, call this function each frame the collision
   *         is happening to notify the actor that is colliding, with a
   *         reference to the other actor involved in this collision.
   * @param    info  The collision information. This has a reference to the
   *           other actor involved in the collision.
   * @bug    No know Bugs
   */
  void onCollisionStay(CollisionInfo info) const;
  /**
   * @brief  If this actor has a collider component active and it was colliding
   *         with another collider las frame but now it isn't, call this
   *         function to notify the actor that the collision ended, with a
   *         reference to the other actor involved in this collision.
   * @param    info  The collision information. This has a reference to the
   *           other actor involved in the collision.
   * @bug    No know Bugs
   */
  void onCollisionExit(CollisionInfo info) const;

 private:
  friend class Scene;

  /**
   * @brief The name of the actor.
   */
  String m_sName;
  /**
   * @brief If the actor is active or not.
   */
  bool m_bActive = true;
  /**
   * @brief If this actor has started its lifetime.
   */
   bool m_bIsStarted;

  /**
   * @brief The list of components.
   */
  Vector<SPtr<Component>> m_vComponents;

  /**
   * @brief The hash index this actor has on the scene.
   *        This if for easy access of the scene.
   */
  int m_iSceneHashIndex;
  /**
   * @brief A reference to the scene this actor is on.
   */
  WPtr<Scene> m_pScene;
};

template<typename T>
FORCEINLINE WPtr<T> Actor::addComponent()
{
  T c; // This is to disable warning "constant logic expression"
  if (c.getType() == eCOMPONENT_TYPE::kTransform) {
  /*auto& loggerMan = Logger::instance();
    loggerMan.consoleLog("Things can't be in more than one place, kido.");
    loggerMan.consoleLog("ERROR ADDING TRANSFORM COMPONENT TO ACTOR.");
    loggerMan.consoleLog("CANNOT ADD A TRANSFORM COMPONENT");*/
    return getComponent<T>();
  }
  SIZE cmpIndex = m_vComponents.size();
  //m_vComponents.push_back(MemoryManager::instance().newPtr<T>());
  m_vComponents.push_back(std::make_shared<T>());
  m_vComponents[cmpIndex]->init(shared_from_this());

  return MemoryManager::sharedReinterpretCast<T>(m_vComponents[cmpIndex]);
}
template<typename T>
FORCEINLINE WPtr<T> Actor::getComponent() const
{
  SIZE compCount = m_vComponents.size();
  for (U32 i = 0; i != compCount; ++i) {
    if (m_vComponents[i]->getType() == T::CmpType) {
      if (T::CmpType == eCOMPONENT_TYPE::kScript) {
        SPtr<T> compCast = MemoryManager::sharedDynamicCast<T>(m_vComponents[i]);
        if (compCast) return compCast;
        continue;
      }
      return MemoryManager::sharedReinterpretCast<T>(m_vComponents[i]);
    }
  }
  return {};
}