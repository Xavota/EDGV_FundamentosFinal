#pragma once

#include "platform/defines.h"
#include "platform/stdHeaders.h"

namespace sf {
  class RenderWindow;
}

class Actor;
class RectCollider;
class CircleCollider;

class Scene : public EnableSPtrFromThis<Scene> {
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
   * @brief Adds an actor to the list of actors, given a name.
   *        If the actor doesn't have a name, the name will be set to "Actor".
   * @param   name  The name of the new actor. If this is left empty, the new
   *                actor will be stored as "Actor". This need NOT to be unqiue,
   *                since the scene will add a hash index to make it unique.
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

  /**
   * @brief Get all the actors of this scene.
   */
  FORCEINLINE const Map<String, SPtr<Actor>>& getActors() { return m_mActors; }

 private:
  /**
   * @brief Initializes the scene.
   */
  void init();
  /**
   * @brief Starts the life of the actors in the scene.
   */
  void start();
  /**
   * @brief Ends the life of the actors in the scene.
   */
  void unstart();
  /**
   * @brief Updates everything in the scene. Calls all the root actors' update
   *        method and they call their children update and so on, and every
   *        actor calls the update of their components.
   */
  void update();
  /**
   * @brief Add the actor children recursively in the rendering actor map for
   *        the render funtion, only if they need to be rendered.
   */
  void addRenderingActorsRecursive(SPtr<Actor> rootActor,
                          Map<int, Vector<SPtr<Actor>>>& renderingActors) const;
  /**
   * @brief Renders everything in the scene. Looks for a render component and a
   *        shape component so it can draw the shape with the material of the
   *        render component.
   * @param   window  The windows for the scene to draw to.
   */
  void render(sf::RenderWindow* window) const;
  /**
   * @brief Renders everything the scene UI. This is render on top of everthing
   *        and it's called after all the normal render of all the scenes.
   * @param   window  The windows for the scene to draw to.
   */
  void renderUI(sf::RenderWindow* window) const;
  /**
   * @brief Destroys the scene. Calls the destroy method of every actor and then
   *        releases the Actors' list as a whole.
   */
  void destroy();

  /**
   * @brief Calls all the actors' destroy method and clears the actors list.
   */
  void clearActors();

  bool checkRectToRectCollision(SPtr<RectCollider> rCollider,
                                SPtr<RectCollider> ROther) const;
  bool checkRectToCircleCollision(SPtr<RectCollider> rCollider,
                                  SPtr<CircleCollider> cOther) const;
  bool checkCircleToCircleCollision(SPtr<CircleCollider> cCollider,
                                    SPtr<CircleCollider> cOther) const;
  /**
   * @brief Checks the collisions of all the actors that have a collider
   *        component active. If two of them are colliding, it calculates the
   *        collision point and normal and sends a notification to both of the
   *        actors with this information and a reference to the actor they are
   *        colliding with.
   */
  void checkCollisions() const;

 private:
  friend class SceneManager;

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

  /**
   * @brief If the scene should render its debug grid, to show the unit size.
   */
   bool m_bRenderDebugGrid;

  /**
   * @brief A list of the names of the actor that are queue to be deleted this
   *        frame.
   */
  Vector<String> m_vPendingDelete;
};