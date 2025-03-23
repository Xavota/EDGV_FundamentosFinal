#include "tools/input.h"

namespace gl {
  Map<sf::Keyboard::Key, Input::ButtonState> Input::mKeyboardKeys;
  Map<sf::Mouse::Button, Input::ButtonState> Input::mMouseButtons;
  
  bool Input::getKeyPressed(sf::Keyboard::Key keyCode)
  {
    const auto it = mKeyboardKeys.find(keyCode);
    if (it == mKeyboardKeys.end()) return false;

    return it->second.mWasPressed ? false : it->second.mIsPressed;
  }
  
  bool Input::getKey(sf::Keyboard::Key keyCode)
  {
    const auto it = mKeyboardKeys.find(keyCode);
    if (it == mKeyboardKeys.end()) return false;

    return it->second.mIsPressed;
  }
  
  bool Input::getKeyReleased(sf::Keyboard::Key keyCode)
  {
    const auto it = mKeyboardKeys.find(keyCode);
    if (it == mKeyboardKeys.end()) return false;

    return it->second.mWasPressed ? !it->second.mIsPressed : false;
  }
  
  bool Input::getMouseButtonDown(sf::Mouse::Button button)
  {
    const auto it = mMouseButtons.find(button);
    if (it == mMouseButtons.end()) return false;

    return it->second.mWasPressed ? false : it->second.mIsPressed;
  }
  
  bool Input::getMouseButton(sf::Mouse::Button button)
  {
    const auto it = mMouseButtons.find(button);
    if (it == mMouseButtons.end()) return false;

    return it->second.mIsPressed;
  }
  
  bool Input::getMouseButtonUp(sf::Mouse::Button button)
  {
    const auto it = mMouseButtons.find(button);
    if (it == mMouseButtons.end()) return false;

    return it->second.mWasPressed ? !it->second.mIsPressed : false;
  }
  
  sf::Vector2i Input::getMousePosition(sf::RenderWindow* window)
  {
    return sf::Mouse::getPosition(*window);
  }
  
  sf::Vector2i Input::getMousePositionInGame(sf::RenderWindow* window)
  {
    const sf::View view = window->getView();
    sf::Vector2i relativePos = getMousePosition(window);

    sf::Vector2u windowSize = window->getSize();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    return sf::Vector2i((viewCenter - viewSize * .5f) +
           sf::Vector2f((relativePos.x * (viewSize.x / windowSize.x)),
           (relativePos.y * (viewSize.y / windowSize.y))));
  }
  
  void Input::handleKeyboardInput(sf::Keyboard::Key keyCode, bool isPressed)
  {
    const auto it = mKeyboardKeys.find(keyCode);
    if (it == mKeyboardKeys.end()) {
      mKeyboardKeys[keyCode] = ButtonState(isPressed);
      return;
    }

    it->second.mIsPressed = isPressed;
  }

  void Input::handleMouseInput(sf::Mouse::Button button, bool isPressed) 
  {
    const auto it = mMouseButtons.find(button);
    if (it == mMouseButtons.end()) {
      mMouseButtons[button] = ButtonState(isPressed);
      return;
    }

    it->second.mIsPressed = isPressed;
  }
  
  void Input::update()
  {
    for (Map<sf::Keyboard::Key, ButtonState>::iterator it = mKeyboardKeys.begin(); it != mKeyboardKeys.end(); ++it) {
      it->second.mWasPressed = it->second.mIsPressed;
    }

    for (Map<sf::Mouse::Button, ButtonState>::iterator it = mMouseButtons.begin(); it != mMouseButtons.end(); ++it) {
      it->second.mWasPressed = it->second.mIsPressed;
    }
  }
}