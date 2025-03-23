#pragma once

#include "scene/component.h"

/**
 * @brief  One of the shape components. This tells the renderer to render a
 *         rectangle with the material and transform the actor has.
 *         This class is just informative to the pipeline, the renderer has a
 *         sf::RectangeShape of its own and use it to render all the ShapeRect
 *         component it founds.
 */
class ShapeRect : public Component {
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kShapeRect };

  /**
   * @brief Default constructor.
   */
  ShapeRect() = default;
  /**
   * @brief Default destructor.
   */
  ~ShapeRect() = default;

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