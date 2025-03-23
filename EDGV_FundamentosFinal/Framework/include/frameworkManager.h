#pragma once

#include <SFML/Graphics.hpp>

#include "platform/stdHeaders.h"
#include "platform/module.hpp"

/// TEMPS
#include "scene/scene.h"

class FrameworkManager : public Module<FrameworkManager> {
 public:
 /**
  * @brief  Default constructor
  */
 FrameworkManager() = default;
  /**
   * @brief  Default destructor
   */
  ~FrameworkManager() = default;

  /**
   * @brief  Start the game loop of the framework. This initiates, updates,
   *         render and, at the end, destroys the whole app
   * @param    size      The size of the game window, in pixels.
   * @param    name      The name of the game window.
   *                     (Will be displayed at the top).
   * @param    windowed  If the game window is in window mode or fullscreen.
   */
  void run(const sf::Vector2u& size = {1024, 1024},
           const String& name = "sfmlWindow",
           const bool windowed = true);

 protected:
  /**
   * @brief  Init function to inherit.
   */
  virtual void onInit() {};
  /**
   * @brief  Update function to inherit.
   */
  virtual void onUpdate() {};
  /**
   * @brief  Render function to inherit.
   */
  virtual void onRender() {};
  /**
   * @brief  Destroy function to inherit.
   */
  virtual void onDestroy() {};

 private:
  /**
   * @brief  Copy constructor private to avoid creation of other instances of
   *         this class, since it's a module
   */
  FrameworkManager(const FrameworkManager& other) = default;

  /**
   * @brief  Initiates the RenderWindow of the framework and stores it in
   *         [m_window]
   * @param    size      The size of the window, in pixels.
   * @param    name      The name of the window. (Will be displayed at the top).
   * @param    windowed  If the window is in window mode or fullscreen.
   */
  void initGameWindow(const sf::Vector2u& size, const String& name,
                      const bool windowed);
  /**
   * @brief  Processes the events of the game window, the keyboard inputs, the
   *         mouse buttons and position, other inputs on controller and
   *         touchpad, and any change in the window, as well as closing the app.
   */
  void processEvents();

  /**
   * @brief  Initializes all the systems in the framework.
   * @param    size      The size of the game window, in pixels.
   * @param    name      The name of the game window.
   *                     (Will be displayed at the top).
   * @param    windowed  If the game window is in window mode or fullscreen.
   */
  void init(const sf::Vector2u& size, const String& name, const bool windowed);
  /**
   * @brief  Updates the framework systems and calls the update of anything in it.
   */
  void update();
  /**
   * @brief  Renders all the render objects and UI.
   */
  void render();
  /**
   * @brief  Releases the memory of anything that needs to.
   */
  void destroy();

 private:
  /**
   * @brief  The game window where everything will be displayed and captures
   *         the game events.
   */
  sf::RenderWindow m_window;
  /**
   * @brief  The fixed time that each logical frame has in between.
   */
  sf::Time m_timePerFrame = sf::seconds(1.f / 60.f);


  /// TEMPS
  Scene m_scene;
};