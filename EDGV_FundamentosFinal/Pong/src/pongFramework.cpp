#include "pongFramework.h"

#include <scene/sceneManager.h>
#include <scene/scene.h>
#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/render.h>
#include <scene/components/shapeCircle.h>
#include <scene/components/shapeRect.h>
#include <scene/components/rectCollider.h>
#include <scene/components/circleCollider.h>
#include "scripts/movementScript.h"


void PongFramework::onInit()
{
  SPtr<Scene> testScene = SceneManager::instance().addScene("TestScene").lock();

  SPtr<Actor> testActor = testScene->addActor("TestActor").lock();
  testActor->addComponent<ShapeCircle>();
  //testActor->addComponent<ShapeRect>();
  SPtr<Render> testActorRender = testActor->addComponent<Render>().lock();
  testActorRender->m_material.m_color = sf::Color::Red;
  SPtr<Transform> testActorTransform = testActor->getTransform().lock();
  testActorTransform->setScale({5.0f, 3.0f});
  testActorTransform->setPosition({15.0f, 8.0f});
  SPtr<RectCollider> testActorColl = testActor->addComponent<RectCollider>().lock();
  //testActor3Coll->offset = {2.0f, 3.0f};
  testActorColl->scale = {1.0f, 1.0f};
  testActorColl->render = true;
  
  SPtr<Actor> testActor2 = testScene->addActor("TestActor2").lock();
  testActor2->addComponent<ShapeCircle>();
  SPtr<Render> testActor2Render = testActor2->addComponent<Render>().lock();
  testActor2Render->m_material.m_color = sf::Color::Blue;
  testActor2Render->m_iRenderIndex = 1;
  SPtr<Transform> testActor2Transform = testActor2->getTransform().lock();
  //testActor2Transform->setScale({0.4f, 0.4f});
  testActor2Transform->setPosition({8.0f, 8.0f});
  SPtr<CircleCollider> testActor2Coll = testActor2->addComponent<CircleCollider>().lock();
  //testActor2Coll->offset = {5.0f, 2.0f};
  testActor2Coll->scaleRadius = 3.0f;
  testActor2Coll->render = true;
  testActor2->addComponent<MovementScript>();
  
  SPtr<Actor> testActor3 = testScene->addActor("TestActor3").lock();
  testActor3->addComponent<ShapeRect>();
  SPtr<Render> testActor3Render = testActor3->addComponent<Render>().lock();
  testActor3Render->m_material.m_color = sf::Color::Green;
  testActor3Render->m_iRenderIndex = 2;
  SPtr<Transform> testActor3Transform = testActor3->getTransform().lock();
  testActor3Transform->setScale({2.0f, 2.0f});
  testActor3Transform->setPosition({8.0f, 8.0f});
  SPtr<RectCollider> testActor3Coll = testActor3->addComponent<RectCollider>().lock();
  //testActor3Coll->offset = {2.0f, 3.0f};
  testActor3Coll->scale = {4.0f, 4.0f};
  testActor3Coll->render = true;
  testActor3Coll->setActive(false);

  SceneManager::instance().openScene("TestScene");
}
void PongFramework::onUpdate()
{

}
void PongFramework::onRender()
{

}
void PongFramework::onDestroy()
{

}