#pragma once

#include "platform/defines.h"
#include "platform/stdHeaders.h"

namespace sf {
  class RenderWindow;
}

class Actor;

class Scene {
 public: 
  /**
   * @brief Default constructor.
   */
  Scene() = default;
  /**
   * @brief Default destructor.
   */
  ~Scene() = default;

  /**
   * @brief Initializes the scene.
   */
  void init();
  /**
   * @brief Updates everything in the scene. Calls all the root actors' update
   *        method and they call their children update and so on, and every
   *        actor calls the update of their components.
   */
  void update();
  /**
   * @brief Renders everything in the scene. Looks for a render component and a
   *        shape component so it can draw the shape with the material of the
   *        render component.
   * @param   window  The windows for the scene to draw to.
   */
  void render(sf::RenderWindow* window);
  /**
   * @brief Destroys the scene. Calls the destroy method of every actor and then
   *        releases the Actors' list as a whole.
   */
  void destroy();

  /**
   * @brief Get all the actors of this scene.
   */
  FORCEINLINE const Map<String, SPtr<Actor>>& getActors() { return m_mActors; }

  /**
   * @brief Adds an actor to the list of actors, given a name.
   *        If the actor doesn't have a name, the name will be set to "Actor".
   * @param   name  The name of the new actor. If this is left empty, the new
   *          actor will be stored as "Actor".
   */
  WPtr<Actor> addActor(const String& name = "");
  /**
   * @brief Searches and returns for the first actor with the given name.
   * @param   name  The name of the actor to be searched for.
   */
  WPtr<Actor> getActor(const String& name) const;
  /**
   * @brief Destroy the given actor and remove it from the scene.
   * @param   actor  The actor to be destroyed. The actor stores a reference of
   *                 it's name and hash, so the scene can get its full name
   *                 easilly.
   */
  void destroyActor(const WPtr<Actor> actor);

 private:
  /**
   * @brief Calls all the actors' destroy method and clears the actors list.
   */
  void clearActors();

 private:
 /**
  * @brief Whether this scene is active or not in the hierachy. Normally only
  *        one scene would be active, but more than one can be active at the
  *        same time.
  */
  bool m_bIsActive;
 /**
  * @brief The actors map of the scene. This is all the actors, without
  *        hierachy, just to have the references to all of them.
  */
  Map<String, SPtr<Actor>> m_mActors;
 /**
  * @brief A hash index for the actors' names, so they don't collide on the map.
  */
  int m_iActorsHashIndex = 0;
};