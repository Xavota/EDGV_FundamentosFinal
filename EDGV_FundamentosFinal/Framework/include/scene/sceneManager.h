#pragma once

#include "platform/module.hpp"

#include "platform/stdHeaders.h"

class Scene;

class SceneManager : public Module<SceneManager> {
 public: 
  /**
   * @brief Default constructor.
   */
  SceneManager() = default;
  /**
   * @brief Default destructor.
   */
  ~SceneManager() = default;

 private:
  /**
   * @brief All the scenes in the game.
   */
  Map<String, SPtr<Scene>> m_mScenes;
};