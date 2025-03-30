#include "frameworkManager.h"

#include "tools/globals.h"

#include "scene/sceneManager.h"
#include "scene/scene.h"

void FrameworkManager::run(const sf::Vector2u& size, const String& name,
                           const bool windowed)
{
  init(size, name, windowed);
  
  auto& time = gl::Time::instance();

  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (m_window.isOpen())
  {
    processEvents();

    timeSinceLastUpdate += clock.restart();
    while (timeSinceLastUpdate >= m_timePerFrame)
    {
      timeSinceLastUpdate -= m_timePerFrame;
      time.update(m_timePerFrame.asSeconds());

      processEvents();
      update();
    }

    render();
  }

  destroy();
}

void FrameworkManager::initGameWindow(const sf::Vector2u& size,
                                      const String& name,
                                      const bool windowed)
{
  m_window = sf::RenderWindow(sf::VideoMode(size), name,
   windowed ? sf::State::Windowed : sf::State::Fullscreen);
}

void FrameworkManager::init(const sf::Vector2u& size, const String& name,
                            const bool windowed)
{
  initGameWindow(size, name, windowed);

  gl::Time::startUp();
  gl::TextureManager::startUp();
  gl::FontManager::startUp();
  SceneManager::startUp();

  SceneManager::instance().init();

  onInit();
}

void FrameworkManager::processEvents()
{
  while (const std::optional event = m_window.pollEvent())
  {
    if (event->is<sf::Event::KeyPressed>()) {
      gl::Input::handleKeyboardInput(event->getIf<sf::Event::KeyPressed>()->code, true);
    }
    else if (event->is<sf::Event::KeyReleased>()) {
      gl::Input::handleKeyboardInput(event->getIf<sf::Event::KeyReleased>()->code, false);
    }
    else if (event->is<sf::Event::MouseButtonPressed>()) {
      gl::Input::handleMouseInput(event->getIf<sf::Event::MouseButtonPressed>()->button, true);
    }
    else if (event->is<sf::Event::MouseButtonReleased>()) {
      gl::Input::handleMouseInput(event->getIf<sf::Event::MouseButtonReleased>()->button, false);
    }
    else if (event->is<sf::Event::Closed>()) {
      m_window.close();
    }
    else {}
  }
}

void FrameworkManager::update() {
  SceneManager::instance().update();

  onUpdate();
}

void FrameworkManager::render() {
  m_window.clear();

  SceneManager::instance().render(&m_window);

  onRender();

  m_window.display();
}

void FrameworkManager::destroy() {
  onDestroy();

  SceneManager::instance().destroy();

  SceneManager::shutDown();
  gl::FontManager::shutDown();
  gl::TextureManager::shutDown();
  gl::Time::shutDown();
}