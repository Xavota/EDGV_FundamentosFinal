#include "frameworkManager.h"

#include "tools/input.h"
#include "tools/time.h"

#include "scene/actor.h"
#include "scene/components/transform.h"
#include "scene/components/render.h"
#include "scene/components/shapeCircle.h"
#include "scene/components/shapeRect.h"

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

  onInit();

  /// TEMPS
  m_scene.init();

  SPtr<Actor> actorTest1 = m_scene.addActor("Test1").lock();
  SPtr<Render> renderTest1 = actorTest1->addComponent<Render>().lock();
  renderTest1->m_material.m_color = sf::Color::Blue;
  actorTest1->addComponent<ShapeRect>();
  //SPtr<Transform> transformTest1 = actorTest1->getTransform().lock();
  //transformTest1->setPosition({2.0f, 2.0f});
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
    else {
      
    }
  }
}

void FrameworkManager::update() {
  onUpdate();

  /// TEMPS
  m_scene.update();
}

void FrameworkManager::render() {
  m_window.clear();

  onRender();

  /// TEMPS
  m_scene.render(&m_window);

  m_window.display();
}

void FrameworkManager::destroy() {
  onDestroy();

  m_scene.destroy();
}