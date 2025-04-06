#include "scene/components/button.h"

#include "frameworkManager.h"

#include "tools/input.h"

#include "scene/components/transform.h"

void Button::update()
{
  auto& framework = FrameworkManager::instance();

  sf::Vector2i mousePos = gl::Input::getMousePosition(framework.getGameWindow());
  bool mousePressedDown = gl::Input::getMouseButtonDown(sf::Mouse::Button::Left);
  bool mousePressed = gl::Input::getMouseButton(sf::Mouse::Button::Left);
  bool mousePressedUp = gl::Input::getMouseButtonUp(sf::Mouse::Button::Left);

  //std::cout << "mousePos(" << mousePos.x << ", " << mousePos.y << ")" << std::endl;

  SPtr<Transform> transform = getTransform().lock();
  sf::Vector2f realScale = transform->getScale() * framework.m_fPixelToMeterSize;
  sf::Vector2f realPos = transform->getPosition() * framework.m_fPixelToMeterSize
                       - realScale * 0.5f;

  sf::FloatRect globalBounds = sf::FloatRect(realPos, realScale);

  if (globalBounds.contains(static_cast<sf::Vector2f>(mousePos))) {
    if (m_bMouseOver) {
      if (m_fpOnHovered != nullptr) {
        m_fpOnHovered();
      }
    }
    else {
      if (m_fpOnMouseEnter != nullptr) {
        m_fpOnMouseEnter();
      }
    }

    m_bMouseOver = true;
  }
  else if (m_bMouseOver) {
    if (m_fpOnMouseExit != nullptr) {
      m_fpOnMouseExit();
    }

    m_bMouseOver = false;
  }

  if (m_bMouseOver) {
    if (mousePressedDown) {
      m_bBeingClicked = true;
      if (m_fpOnClick != nullptr) {
        m_fpOnClick();
      }
    }
    else if (m_bBeingClicked && mousePressed) {
      if (m_fpOnClickContinue != nullptr) {
        m_fpOnClickContinue();
      }
    }
    else if (m_bBeingClicked && mousePressedUp) {
      if (m_fpOnClickStops != nullptr) {
        m_fpOnClickStops();
      }
    }
  }
  else {
    m_bBeingClicked = false;
  }
}