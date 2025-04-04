#include "scene/scene.h"

#include <SFML/Graphics.hpp>

#include "platform/memoryManager.hpp"
#include "platform/math.h"

#include "scene/actor.h"
#include "scene/components/transform.h"
#include "scene/components/render.h"
#include "scene/components/shapeRect.h"
#include "scene/components/shapeCircle.h"
#include "scene/components/circleCollider.h"
#include "scene/components/rectCollider.h"

WPtr<Actor> Scene::addActor(const String& name)
{
  String internalName = name.empty() ? "Actor" : name;
  String newName = internalName + "_" + std::to_string(m_iActorsHashIndex);

  m_mActors[newName] = MemoryManager::createUnique<Actor>();
  m_mActors[newName]->init(internalName, m_iActorsHashIndex);
  m_mActors[newName]->m_pScene = shared_from_this();

  if (m_bIsActive) {
    m_mActors[newName]->start();
  }

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
    m_vPendingDelete.push_back(actorName);
  }

  // Log an error that the actor was not found on this scene.
}


void Scene::init()
{
  clearActors();

  m_bIsActive = false;
  m_mActors = {};
  m_iActorsHashIndex = 0;
}

void Scene::start()
{
  for (auto& actor : m_mActors) {
    if (!actor.second->isActive()) continue;

    actor.second->start();
  }
}

void Scene::unstart()
{
  for (auto& actor : m_mActors) {
    actor.second->m_bIsStarted = false;
  }
}

void Scene::update()
{
  checkCollisions();

  for (auto& actor : m_mActors) {
    if (!actor.second->isActive()) continue;

    actor.second->update();
  }

  
  // Delete pending delete actors
  for (const String& a : m_vPendingDelete) {
    auto actorFound = m_mActors.find(a);
    if (actorFound != m_mActors.end()) {
      actorFound->second->destroy();
      m_mActors.erase(actorFound);
    }
  }
  m_vPendingDelete.clear();
}

void Scene::render(sf::RenderWindow* window) const
{
  if (nullptr == window) return;

  sf::RectangleShape rectangleShape;
  sf::CircleShape circleShape;

  // Render grid
  if (m_bRenderDebugGrid) {
    rectangleShape.setSize({1.0f, 1024.0f});
    rectangleShape.setFillColor(sf::Color(50, 50, 50));
    for (int i = 0; i < 100; ++i) {
      rectangleShape.setPosition({m_fPixelToMeterSize * i, 0.0f});
      window->draw(rectangleShape);
    }
    rectangleShape.setSize({1024.0f, 1.0f});
    for (int i = 0; i < 100; ++i) {
      rectangleShape.setPosition({0.0f, m_fPixelToMeterSize * i});
      window->draw(rectangleShape);
    }
  }


  Map<int, Vector<SPtr<Actor>>> renderingActors;
  for (auto& actor : m_mActors) {
    if (!actor.second->isActive()) continue;

    WPtr<Render> renderComp = actor.second->getComponent<Render>();
    WPtr<ShapeRect> rectComp = actor.second->getComponent<ShapeRect>();
    WPtr<ShapeCircle> circleComp = actor.second->getComponent<ShapeCircle>();
    if ((renderComp.expired() || !renderComp.lock()->getActive()) ||
       ((rectComp.expired()   || !rectComp.lock()->getActive()) &&
        (circleComp.expired() || !circleComp.lock()->getActive()))) continue;

    int renderIndex = renderComp.lock()->m_iRenderIndex;
    if (renderingActors.find(renderIndex) == renderingActors.end()) {
      renderingActors[renderIndex] = {};
    }
    renderingActors[renderIndex].push_back(actor.second);
  }

  for (auto& actorsLayer : renderingActors) {
    for (auto& actor : actorsLayer.second) {
      SPtr<Transform> transform = actor->getComponent<Transform>().lock();
      SPtr<Render> renderComp = actor->getComponent<Render>().lock();
      WPtr<ShapeRect> rectComp = actor->getComponent<ShapeRect>();
      WPtr<ShapeCircle> circleComp = actor->getComponent<ShapeCircle>();
  
      sf::Vector2f pos = transform->getPosition() * m_fPixelToMeterSize;
      sf::Angle rot = transform->getRotation();
      sf::Vector2f scale = transform->getScale() * m_fPixelToMeterSize;
  
      if (!rectComp.expired()) {
        rectangleShape.setPosition(pos);
        rectangleShape.setSize(scale);
        rectangleShape.setOrigin(scale * 0.5f);
        rectangleShape.setRotation(rot);
  
        if (nullptr != renderComp->m_material.m_pTexture) {
          rectangleShape.setTexture(renderComp->m_material.m_pTexture);
          rectangleShape.setTextureRect(
            sf::IntRect(
              {0,0},
              static_cast<sf::Vector2i>(renderComp->m_material.m_pTexture->getSize())
            )
          );
        }
        else {
          rectangleShape.setTexture(nullptr);
        }
        rectangleShape.setFillColor(renderComp->m_material.m_color);
  
        window->draw(rectangleShape);
      }
      if (!circleComp.expired()) {  
        sf::Vector2f newScale = scale * 0.5f;
        float radius = std::max(std::abs(newScale.x), std::abs(newScale.y));

        circleShape.setPosition(pos);
        circleShape.setRadius(radius);
        circleShape.setOrigin({radius, radius});
        circleShape.setScale(newScale / radius);
        circleShape.setRotation(rot);
  
        if (nullptr != renderComp->m_material.m_pTexture) {
          circleShape.setTexture(renderComp->m_material.m_pTexture);
          rectangleShape.setTextureRect(
            sf::IntRect(
              {0,0},
              static_cast<sf::Vector2i>(renderComp->m_material.m_pTexture->getSize())
            )
          );
        }
        else {
          rectangleShape.setTexture(nullptr);
        }
        circleShape.setFillColor(renderComp->m_material.m_color);
  
        window->draw(circleShape);
      }
    }
  }
}

void Scene::renderUI(sf::RenderWindow* window) const
{
  sf::RectangleShape rectangleShape;
  sf::CircleShape circleShape;

  // DEBUG ONLY. RENDER THE COLLIDER AREAS IF THEY NEED TO.
  for (auto& actor : m_mActors) {
    WPtr<RectCollider> rectColl =
     actor.second->getComponent<RectCollider>();
    WPtr<CircleCollider> circleColl =
     actor.second->getComponent<CircleCollider>();
    if ((rectColl.expired() ||
         !rectColl.lock()->getActive() ||
         !rectColl.lock()->render) &&
        (circleColl.expired() ||
         !circleColl.lock()->getActive() ||
         !circleColl.lock()->render)) continue;

    SPtr<Transform> transform = actor.second->getComponent<Transform>().lock();
    sf::Angle rot = transform->getRotation();
    sf::Vector2f scale = transform->getScale() * m_fPixelToMeterSize;

    if (!rectColl.expired() &&
         rectColl.lock()->getActive() &&
         rectColl.lock()->render) {
      SPtr<RectCollider> rCollider = rectColl.lock();

      sf::Vector2f cScale = rCollider->scale;
      sf::Vector2f newScale = {scale.x * cScale.x, scale.y * cScale.y};

      rectangleShape.setPosition(rCollider->m_currentGlobalPosition);
      rectangleShape.setSize(newScale);
      rectangleShape.setOrigin(newScale * 0.5f);
      rectangleShape.setRotation(rot);

      rectangleShape.setFillColor(sf::Color(30, 255, 80, 50));
      rectangleShape.setOutlineColor(sf::Color(30, 200, 80, 200));
      rectangleShape.setOutlineThickness(1.0f);

      window->draw(rectangleShape);

      circleShape.setPosition(rCollider->m_currentGlobalPosition);
      circleShape.setRadius(rCollider->m_boundingRadius);
      circleShape.setOrigin({rCollider->m_boundingRadius, rCollider->m_boundingRadius});
      circleShape.setRotation(sf::degrees(0.0f));

      circleShape.setFillColor(sf::Color::Transparent);
      circleShape.setOutlineColor(sf::Color(255, 255, 255, 100));
      circleShape.setOutlineThickness(1.0f);

      window->draw(circleShape);
    }
    if (!circleColl.expired() &&
         circleColl.lock()->getActive() &&
         circleColl.lock()->render) {
      SPtr<CircleCollider> cCollider = circleColl.lock();

      sf::Vector2f offset = cCollider->offset;
      sf::Vector2f newPos =
       transform->getModelMatrix().transform(offset) * m_fPixelToMeterSize;

       float cScaleRadius = cCollider->scaleRadius;
       float radius = std::max(scale.x, scale.y) * cScaleRadius;

      circleShape.setPosition(newPos);
      circleShape.setRadius(radius);
      circleShape.setOrigin({radius, radius});
      circleShape.setRotation(rot);

      circleShape.setFillColor(sf::Color(30, 255, 80, 50));
      circleShape.setOutlineColor(sf::Color(30, 200, 80, 200));
      circleShape.setOutlineThickness(1.0f);

      window->draw(circleShape);
    }
  }
}

void Scene::destroy()
{
  clearActors();
}


void Scene::clearActors()
{
  for (auto& actor : m_mActors) {
    actor.second->destroy();
  }

  m_mActors.clear();
}


bool Scene::checkRectToRectCollision(SPtr<RectCollider> rCollider,
                                     SPtr<RectCollider> rOther) const
{
  float maxDistance = rCollider->m_boundingRadius + rOther->m_boundingRadius;
  float distance = Math::distanceVector(rCollider->m_currentGlobalPosition,
                                        rOther->m_currentGlobalPosition);

  // If the are not even close, skip collision calculation
  if (distance > maxDistance) return false;

  sf::Vector2f tlPosA = rCollider->m_currentGlobalPosition -
                        rCollider->m_globalScale * 0.5f;
  sf::FloatRect rectA = sf::FloatRect(tlPosA, rCollider->m_globalScale);
  sf::Vector2f tlPosB = rOther->m_currentGlobalPosition -
                        rOther->m_globalScale * 0.5f;
  sf::FloatRect rectB = sf::FloatRect(tlPosB, rOther->m_globalScale);

  return rectA.findIntersection(rectB) != std::nullopt;
}
bool Scene::checkRectToCircleCollision(SPtr<RectCollider> rCollider,
                                       SPtr<CircleCollider> cOther) const
{
  float maxDistance = rCollider->m_boundingRadius + cOther->m_fRealRadius;
  float distance = Math::distanceVector(rCollider->m_currentGlobalPosition,
                                        cOther->m_currentGlobalPosition);

  // If the are not even close, skip collision calculation
  if (distance > maxDistance) return false;
  
  sf::Vector2f rectRelativePos =
   rCollider->m_currentGlobalPosition - cOther->m_currentGlobalPosition;
  sf::Vector2f halfScale = rCollider->m_globalScale * 0.5f;

  sf::Vector2f A = rectRelativePos +
   sf::Vector2f{halfScale.x, halfScale.y};
  sf::Vector2f B = rectRelativePos +
   sf::Vector2f{-halfScale.x, halfScale.y};
  sf::Vector2f C = rectRelativePos +
   sf::Vector2f{-halfScale.x, -halfScale.y};
  sf::Vector2f D = rectRelativePos +
   sf::Vector2f{halfScale.x, -halfScale.y};

  // If any of the rect's corners are inside the circle, both are colliding
  float distA = A.length();
  if (distA <= cOther->m_fRealRadius) return true;
  float distB = B.length();
  if (distB <= cOther->m_fRealRadius) return true;
  float distC = C.length();
  if (distC <= cOther->m_fRealRadius) return true;
  float distD = D.length();
  if (distD <= cOther->m_fRealRadius) return true;

  auto getQuad = [](sf::Vector2f p) -> int {
    return p.y >= 0.0f ? (p.x >= 0.0f ? 1 : 2) : (p.x >= 0.0f ? 4 : 3);
  };
  int quadA = getQuad(A);
  int quadB = getQuad(B);
  int quadC = getQuad(C);
  int quadD = getQuad(D);

  // If all of the rect's corners are in different quadrants of the relative
  // plane, they are colliding
  if (quadA == 1 && quadB == 2 && quadC == 3 && quadD == 4) return true;

  // If 2 corners are inside the bounds of one axis and both of them are not in
  // the same quadrant, they are colliding
  if (std::abs(A.y) <= cOther->m_fRealRadius && quadA != quadB) return true;
  if (std::abs(C.y) <= cOther->m_fRealRadius && quadC != quadD) return true;
  if (std::abs(B.x) <= cOther->m_fRealRadius && quadB != quadC) return true;
  if (std::abs(A.x) <= cOther->m_fRealRadius && quadA != quadD) return true;

  return false;
}

bool Scene::checkCircleToCircleCollision(SPtr<CircleCollider> cCollider,
                                         SPtr<CircleCollider> cOther) const
{
  float maxDistance = cCollider->m_fRealRadius + cOther->m_fRealRadius;
  float distance = Math::distanceVector(cCollider->m_currentGlobalPosition,
                                        cOther->m_currentGlobalPosition);

  return distance <= maxDistance;
}

void Scene::checkCollisions() const
{
  Vector<SPtr<Collider>> colliders = {};

  for (auto& actor : m_mActors) {
    WPtr<RectCollider> rectColl =
     actor.second->getComponent<RectCollider>();
    WPtr<CircleCollider> circleColl =
     actor.second->getComponent<CircleCollider>();

    SPtr<Transform> transform = actor.second->getComponent<Transform>().lock();
    sf::Vector2f scale = transform->getScale() * m_fPixelToMeterSize;

    if (!rectColl.expired() && rectColl.lock()->getActive()) {
      SPtr<RectCollider> rCollider = rectColl.lock();

      sf::Vector2f offset = rCollider->offset;
      sf::Vector2f newPos =
       transform->getModelMatrix().transform(offset) * m_fPixelToMeterSize;

      sf::Vector2f cScale = rCollider->scale;
      sf::Vector2f newScale = {scale.x * cScale.x, scale.y * cScale.y};

      rCollider->m_lastGlobalPosition = rCollider->m_currentGlobalPosition;
      rCollider->m_currentGlobalPosition = newPos;
      rCollider->m_globalScale = newScale;
      rCollider->m_boundingRadius = newScale.length() * 0.5f;

      colliders.push_back(rCollider);
    }
    if (!circleColl.expired() && circleColl.lock()->getActive()) {
      SPtr<CircleCollider> cCollider = circleColl.lock();

      sf::Vector2f offset = cCollider->offset;
      sf::Vector2f newPos =
       transform->getModelMatrix().transform(offset) * m_fPixelToMeterSize;

      cCollider->m_lastGlobalPosition = cCollider->m_currentGlobalPosition;
      cCollider->m_currentGlobalPosition = newPos;

      float cScaleRadius = cCollider->scaleRadius;
      float newRadius = std::max(scale.x, scale.y) * cScaleRadius;
      cCollider->m_fRealRadius = newRadius;

      colliders.push_back(cCollider);
    }
  }

  int collidersCount = static_cast<int>(colliders.size());
  for (int i = 0; i < collidersCount; ++i) {
    SPtr<Collider> collider = colliders[i];
    for (int j = i + 1; j < collidersCount; ++j) {
      SPtr<Collider> other = colliders[j];

      SPtr<Actor> cActor = collider->getActor().lock();
      SPtr<Actor> coActor = other->getActor().lock();

      bool areColliding = false;

      if (collider->getType() == eCOMPONENT_TYPE::kRectCollision) {
        SPtr<RectCollider> rCollider =
         MemoryManager::sharedReinterpretCast<RectCollider>(collider);

        if (other->getType() == eCOMPONENT_TYPE::kRectCollision) {
           SPtr<RectCollider> rOther =
            MemoryManager::sharedReinterpretCast<RectCollider>(other);

            areColliding = checkRectToRectCollision(rCollider, rOther);
        }
        else if (other->getType() == eCOMPONENT_TYPE::kCircleCollision) {
           SPtr<CircleCollider> cOther =
            MemoryManager::sharedReinterpretCast<CircleCollider>(other);

           areColliding = checkRectToCircleCollision(rCollider, cOther);
        }
      }
      else if (collider->getType() == eCOMPONENT_TYPE::kCircleCollision) {
        SPtr<CircleCollider> cCollider =
         MemoryManager::sharedReinterpretCast<CircleCollider>(collider);

        if (other->getType() == eCOMPONENT_TYPE::kRectCollision) {
          SPtr<RectCollider> rOther =
           MemoryManager::sharedReinterpretCast<RectCollider>(other);

          areColliding = checkRectToCircleCollision(rOther, cCollider);
        }
        else if (other->getType() == eCOMPONENT_TYPE::kCircleCollision) {
          SPtr<CircleCollider> cOther =
           MemoryManager::sharedReinterpretCast<CircleCollider>(other);

           areColliding = checkCircleToCircleCollision(cCollider, cOther);
        }
      }
    
      if (areColliding) {
        bool wereColliding = false;
        for (auto& act : other->m_vCollidingActors) {
          if (act.lock()->m_sName == cActor->m_sName &&
              act.lock()->m_iSceneHashIndex == cActor->m_iSceneHashIndex) {
            cActor->onCollisionStay(CollisionInfo{coActor});
            coActor->onCollisionStay(CollisionInfo{cActor});

            wereColliding = true;

            break;
          }
        }

        if (!wereColliding) {
          cActor->onCollisionEnter(CollisionInfo{coActor});
          coActor->onCollisionEnter(CollisionInfo{cActor});

          collider->m_vCollidingActors.push_back(coActor);
          other->m_vCollidingActors.push_back(cActor);
        }
      }
      else {
        SPtr<Actor> cActor = collider->getActor().lock();
        SPtr<Actor> coActor = other->getActor().lock();

        for (Vector<WPtr<Actor>>::iterator it = other->m_vCollidingActors.begin();
             it != other->m_vCollidingActors.end(); ++it) {
          if (it->lock()->m_sName == cActor->m_sName &&
              it->lock()->m_iSceneHashIndex == cActor->m_iSceneHashIndex) {
            cActor->onCollisionExit(CollisionInfo{coActor});
            coActor->onCollisionExit(CollisionInfo{cActor});

            auto ito =
            std::find_if(collider->m_vCollidingActors.begin(),
                         collider->m_vCollidingActors.end(),
                         [&coActor](WPtr<Actor> a)
                         {
                           return a.lock()->m_sName == coActor->m_sName &&
                                  a.lock()->m_iSceneHashIndex == coActor->m_iSceneHashIndex;
                         });

            other->m_vCollidingActors.erase(it);
            collider->m_vCollidingActors.erase(ito);

            break;
          }
        }
      }
    }
  }
}