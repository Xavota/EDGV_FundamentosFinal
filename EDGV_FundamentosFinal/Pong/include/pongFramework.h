#pragma once

#include <frameworkManager.h>

class PongFramework : public FrameworkManager {
 public:
  /**
   * @brief  Default constructor
   */
  PongFramework() = default;
  /**
   * @brief  Default destructor
   */
  ~PongFramework() = default;

 protected:
  /**
   * @brief  Init function to inherit.
   */
  void onInit() override;
  /**
   * @brief  Update function to inherit.
   */
  void onUpdate() override;
  /**
   * @brief  Render function to inherit.
   */
  void onRender() override;
  /**
   * @brief  Destroy function to inherit.
   */
  void onDestroy() override;
};