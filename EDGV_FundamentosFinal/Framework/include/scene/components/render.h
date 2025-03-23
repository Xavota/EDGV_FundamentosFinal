#pragma once

#include "scene/component.h"

#include "resources/material.h"

class Render : public Component
{
 public:
  /**
   * @brief  The enum for identifying the component type.
   */
  enum { CmpType = eCOMPONENT_TYPE::kRender };

  /**
   * @brief Default constructor.
   */
  Render() = default;
  /**
   * @brief Default destructor.
   */
  ~Render() = default;

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
   * @brief The material to render the object with.
   */
  Material m_material;
  /**
   * @brief And index for render priority. Higher index means it renders later,
   *        so it shows in front of renders with lower index.
   */
  int m_iRenderIndex;
};