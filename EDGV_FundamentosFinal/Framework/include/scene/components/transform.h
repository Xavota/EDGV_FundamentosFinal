#pragma once
#include "scene/component.h"

#include <SFML/Graphics.hpp>

#include "platform/defines.h"
#include "platform/stdHeaders.h"
#include "platform/math/matrix3.h"


/**
 * @brief  The transform component.
 */
class Transform : public Component
{
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kTransform };

  /**
   * @brief Default constructor.
   */
  Transform() : m_position({0.0f, 0.0f}), m_rotation(sf::degrees(0.0f)),
    m_scale({1.0f, 1.0f}) {}
  /**
   * @brief Default destructor.
   */
  ~Transform() = default;

  /**
   * @brief   Returns the component type.
   *
   * @return  The component type.
   */
  U8 getType() const override
  {
    return CmpType;
  }

  /**
   * @brief   Returns the local model matrix of the transformations.
   * @return  The local model matrix of the transformations.
   */
  Matrix3 getLocalModelMatrix() const;
  /**
   * @brief   Returns the global model matrix of the transformations.
   * @return  The global model matrix of the transformations.
   */
  Matrix3 getModelMatrix() const;

  /**
   * @brief   Returns the local position of the object.
   * @return  The local position of the object.
   */
  sf::Vector2f getLocalPosition() const;
  /**
   * @brief   Returns the world position of the object.
   * @return  The world position of the object.
   */
  sf::Vector2f getPosition() const;
  /**
   * @brief  Sets the new local position of this transform
   * @param    pos  The new local position for the object.
   */
  void setLocalPosition(const sf::Vector2f& pos);
  /**
   * @brief  Sets the new global position of this transform
   * @param    pos  The new global position for the object.
   */
  void setPosition(const sf::Vector2f& pos);

  /**
   * @brief   Returns the local rotation of the object.
   * @return  The local rotation of the object.
   */
  sf::Angle getLocalRotation() const;
  /**
   * @brief   Returns the global rotation of the object.
   * @return  The global rotation of the object.
   */
  sf::Angle getRotation() const;
  /**
   * @brief  Sets the new local rotation for the object
   * @param     rot  The new local rotation for the object.
   */
  void setLocalRotation(const sf::Angle& rot);
  /**
   * @brief  Sets the new global rotation for the object
   * @param     rot  The new global rotation for the object.
   */
  void setRotation(const sf::Angle& rot);

  /**
   * @brief   Returns the local scale of the object.
   * @return  The local scale of the object.
   */
  sf::Vector2f getLocalScale() const;
  /**
   * @brief   Returns the global scale of the object.
   * @return  The global scale of the object.
   */
  sf::Vector2f getScale() const;
  /**
   * @brief  Sets the new local scale for the object
   * @param    scale  The new local scale for the object.
   */
  void setLocalScale(const sf::Vector2f& scale);
  /**
   * @brief  Sets the new global scale for the object
   * @param    scale  The new global scale for the object.
   */
  void setScale(const sf::Vector2f& scale);


  /**
   * @brief  Attach the transform to a parent transform to adopt the
   *         transformations.
   * @param    transformParent  The new parent transform.
   * @param    retransform      Whether the transform local values shoud change
   *                            to maintain its global values or should they not
   *                            change so the global values change according to
   *                            the new parent values.
   */
  void attachTo(const WPtr<Transform> transformParent, const bool retransform = true);
  /**
   * @brief  Returns the parent of the transform.
   */
  WPtr<Transform> getParent() const { return m_pParent; }

 private:
  /**
   * The position of the actor.
   */
  sf::Vector2f m_position;
  /**
   * The rotation of the actor.
   */
  sf::Angle m_rotation;
  /**
   * The scale of the actor.
   */
  sf::Vector2f m_scale;

  /**
   * The transform component parent.
   */
  WPtr<Transform> m_pParent;
};
