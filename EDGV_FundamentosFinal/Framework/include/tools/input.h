#pragma once
#include <SFML/Graphics.hpp>

#include "platform/defines.h"
#include "platform/stdHeaders.h"

namespace gl {
  /**
   * @brief  Keeps track of all the inputs and store its values to be accesed in every part of the document
   */
  class Input
  {
   public:
    /**
     * @brief  Returns true if the key was pressed in this update
     * @param    keyCode: the key checked
     * @bug    No know Bugs
     */
    static bool getKeyPressed(sf::Keyboard::Key keyCode);
    /**
     * @brief  Returns true if the key is being pressed in this update
     * @param    keyCode: the key checked
     * @bug    No know Bugs
     */
    static bool getKey(sf::Keyboard::Key keyCode);
    /**
     * @brief  Returns true if the key was pressed on the last update and not in this
     * @param    keyCode: the key checked
     * @bug    No know Bugs
     */
    static bool getKeyReleased(sf::Keyboard::Key keyCode);
    /**
     * @brief  Returns true if the mouse button was pressed in this update
     * @param    keyCode: the key checked
     * @bug    No know Bugs
     */
    static bool getMouseButtonDown(sf::Mouse::Button button);
    /**
     * @brief  Returns true if the mouse button is being pressed in this update
     * @param    keyCode: the key checked
     * @bug    No know Bugs
     */
    static bool getMouseButton(sf::Mouse::Button button);
    /**
     * @brief  Returns true if the mouse button was pressed on the last update and not in this
     * @param    keyCode: the key checked
     * @bug    No know Bugs
     */
    static bool getMouseButtonUp(sf::Mouse::Button button);

    /**
     * @brief  Returns the position of the mouse in screen space, from 0 to 1, if it's inside and outside that range if it's outside
     * @param    window: the window to get the position relative to
     * @bug    No know Bugs
     */
    static sf::Vector2i getMousePosition(sf::RenderWindow* window);

    /**
     * @brief  Returns the position of the mouse in game space. (0,0) being the center of the world and getting its distance to that point in pixels.
     *         Also takes in consideration the zoom of the windows view.
     * @param    window: the window to get the position relative to
     * @bug    No know Bugs
     */
    static sf::Vector2i getMousePositionInGame(sf::RenderWindow* window);
  
    /**
     * @brief  Check the changes on the keys beign pressed or not
     * @param    keyCode: the key checked
     * @param    isPressed: if the key is beign pressed or not
     * @bug    No know Bugs
     */
    static void handleKeyboardInput(sf::Keyboard::Key keyCode, bool isPressed);
    /**
     * @brief  Check the changes on the mouse buttons beign pressed or not
     * @param    button: the button checked
     * @param    isPressed: if the button is beign pressed or not
     * @bug    No know Bugs
     */
    static void handleMouseInput(sf::Mouse::Button button, bool isPressed);
    /**
     * @brief  Updates the 'wasPressed' property, for the Pressed and Release functions
     * @bug    No know Bugs
     */
    static void update();
  
   private:
    /**
     * @brief  The data needed to keep track of the button inputs
     */
    class ButtonState {
     public:
      ButtonState() : mIsPressed(false) {}
      ButtonState(bool isPressed) : mIsPressed(isPressed) {}
      
      /**
       * @brief  If the key is pressed in this frame
       */
      bool mIsPressed = false;
      /**
       * @brief  If the key was pressed last frame
       */
      bool mWasPressed = false;
    };

    /**
     * @brief  The keyboard keys states
     */
    static Map<sf::Keyboard::Key, ButtonState> mKeyboardKeys;
    /**
     * @brief  The mouse buttons states
     */
    static Map<sf::Mouse::Button, ButtonState> mMouseButtons;
  };
}
