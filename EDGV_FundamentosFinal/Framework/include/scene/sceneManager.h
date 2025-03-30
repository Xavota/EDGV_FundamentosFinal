#pragma once

#include "platform/module.hpp"

#include "platform/stdHeaders.h"

namespace sf {
  class RenderWindow;
}

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

  /**
   * @brief Adds a new empty scene with the given name.
   * @param   name  The name for the new scene. This should be a unique name no
   *                other scene has already, because it will be used to identify
   *                it later.
   */
  WPtr<Scene> addScene(const String& name);
  /**
   * @brief Adds a new empty scene with the given name.
   * @param   name  The name of the scene to look for.
   */
  WPtr<Scene> getScene(const String& name) const;
  /**
   * @brief Adds a new empty scene with the given name.
   * @param   name  The name of the scene to delete.
   */
  void deleteScene(const String& name);

  /**
   * @brief Get all the scenes of this manager.
   */
  FORCEINLINE const Map<String, SPtr<Scene>>& getScenes() const { return m_mScenes; }

  /**
   * @brief Starts the given scene and de-activates every other scene.
   */
  void openScene(const String& name);

 private:
   /**
    * @brief Initializes the scene manager.
    */
   void init();
   /**
    * @brief Updates everything in the manager. Calls all the scenes' update
    *        methods and they call their actors' update and so on.
    */
   void update();
   /**
    * @brief Renders everything in the active scenes. Calls all the scenes'
    *        render methods
    * @param   window  The windows for the manager to draw to.
    */
   void render(sf::RenderWindow* window) const;
   /**
    * @brief Destroys all the scenes and clear the scene map.
    */
   void destroy();

 private:
  friend class FrameworkManager;

  /**
   * @brief All the scenes in the game.
   */
  Map<String, SPtr<Scene>> m_mScenes;
};