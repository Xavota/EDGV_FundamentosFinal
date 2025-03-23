#pragma once

#include "scene/component.h"

/**
 * @brief  One of the shape components. This tells the renderer to render a
 *         circle with the material and transform the actor has.
 *         This class is just informative to the pipeline, the renderer has a
 *         sf::CircleShape of its own and use it to render all the ShapeCircle
 *         component it founds.
 */
class ShapeCircle : public Component {
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kShapeCircle };

  /**
   * @brief Default constructor.
   */
  ShapeCircle() = default;
  /**
   * @brief Default destructor.
   */
  ~ShapeCircle() = default;

  /**
   * @brief   Returns the component type.
   *
   * @return  The component type.
   */
  U8 getType() const override
  {
    return CmpType;
  }
};