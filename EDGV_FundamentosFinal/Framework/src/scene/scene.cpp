#include "scene/scene.h"

#include <SFML/Graphics.hpp>

#include "platform/memoryManager.hpp"

#include "scene/actor.h"
#include "scene/components/transform.h"
#include "scene/components/render.h"
#include "scene/components/shapeRect.h"
#include "scene/components/shapeCircle.h"

void Scene::init()
{
  clearActors();

  m_iActorsHashIndex = 0;
}

void Scene::update()
{
  for (auto& actor : m_mActors) {
    actor.second->update();
  }
}

void Scene::render(sf::RenderWindow* window)
{
  if (nullptr == window) return;

  Map<int, Vector<SPtr<Actor>>> renderingActors;
  for (auto& actor : m_mActors) {
    if (!actor.second->isActive()) continue;

    WPtr<Render> renderComp = actor.second->getComponent<Render>();
    WPtr<ShapeRect> rectComp = actor.second->getComponent<ShapeRect>();
    WPtr<ShapeCircle> circleComp = actor.second->getComponent<ShapeCircle>();
    if (renderComp.expired() || (rectComp.expired() && circleComp.expired())) continue;

    int renderIndex = renderComp.lock()->m_iRenderIndex;
    if (renderingActors.find(renderIndex) == renderingActors.end()) {
      renderingActors[renderIndex] = {};
    }
    renderingActors[renderIndex].push_back(actor.second);
  }


  sf::RectangleShape rectangleShape;
  sf::CircleShape circleShape;

  for (auto& actorsLayer : renderingActors) {
    for (auto& actor : actorsLayer.second) {
  
      SPtr<Transform> transform = actor->getComponent<Transform>().lock();
      SPtr<Render> renderComp = actor->getComponent<Render>().lock();
      WPtr<ShapeRect> rectComp = actor->getComponent<ShapeRect>();
      WPtr<ShapeCircle> circleComp = actor->getComponent<ShapeCircle>();
  
      sf::Vector2f pos = transform->getPosition() * 10.0f;
      sf::Angle rot = transform->getRotation();
      sf::Vector2f scale = transform->getScale() * 10.0f;
  
      if (!rectComp.expired()) {
        rectangleShape.setPosition(pos);
        rectangleShape.setSize(scale);
        rectangleShape.setOrigin(scale * 0.5f);
        rectangleShape.setRotation(rot);
  
        if (nullptr != renderComp->m_material.m_pTexture) {
          rectangleShape.setTexture(renderComp->m_material.m_pTexture);
        }
        rectangleShape.setFillColor(renderComp->m_material.m_color);
  
        window->draw(rectangleShape);
      }
      if (!circleComp.expired()) {
        float radius = scale.x;
        sf::Vector2f origin = sf::Vector2f{radius, radius} * 0.5f;
  
        circleShape.setPosition(pos);
        circleShape.setRadius(radius);
        circleShape.setOrigin(origin);
        circleShape.setRotation(rot);
  
        if (nullptr != renderComp->m_material.m_pTexture) {
          circleShape.setTexture(renderComp->m_material.m_pTexture);
        }
        circleShape.setFillColor(renderComp->m_material.m_color);
  
        window->draw(circleShape);
      }
    }
  }
}

void Scene::destroy()
{
  clearActors();
}

WPtr<Actor> Scene::addActor(const String& name)
{
  String internalName = name.empty() ? "Actor" : name;
  String newName = internalName + "_" + std::to_string(m_iActorsHashIndex);

  m_mActors[newName] = MemoryManager::createUnique<Actor>();
  m_mActors[newName]->init(internalName, m_iActorsHashIndex);

  ++m_iActorsHashIndex;

  return m_mActors[newName];
}

WPtr<Actor> Scene::getActor(const String& name) const
{
  auto actorsEnd = m_mActors.end();
  for (int i = 0; i < m_iActorsHashIndex; ++i) {
    auto actorFound = m_mActors.find(name + "_" + std::to_string(i));
    if (actorFound != actorsEnd) {
      return actorFound->second;
    }
  }

  return {};
}

void Scene::destroyActor(const WPtr<Actor> actor)
{
  SPtr<Actor> actorLock = actor.lock();
  String actorName = actorLock->m_sName + "_"
                   + std::to_string(actorLock->m_iSceneHashIndex);

  auto actorFound = m_mActors.find(actorName);
  if (actorFound != m_mActors.end()) {
    actorFound->second->destroy();
    m_mActors.erase(actorFound);
  }

  // Log and error that the actor was not found on this scene.
}

void Scene::clearActors()
{
  for (auto& actor : m_mActors) {
    actor.second->destroy();
  }

  m_mActors.clear();
}