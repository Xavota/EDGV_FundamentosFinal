#include "scripts/mapTilesFactory.h"

#include <scene/actor.h>
#include <scene/scene.h>
#include <scene/components/transform.h>
#include <scene/components/shapeRect.h>
#include <scene/components/render.h>

SPtr<Actor> MapTilesFactory::factory(WPtr<Scene> scene,
                                     const sf::Vector2u& coord,
                                     const String& materialName,
                                     WPtr<Transform> parent)
{
  String tileName =
   "t(" + std::to_string(coord.x) + "," + std::to_string(coord.y) + ")";
  SPtr<Actor> tile = scene.lock()->addActor(tileName).lock();

  //std::cout << tileName << " " << materialName << std::endl;

  SPtr<Transform> transform = tile->getTransform().lock();
  transform->attachTo(parent);

  transform->setLocalPosition(static_cast<const sf::Vector2f>(coord));
  tile->addComponent<ShapeRect>();
  SPtr<Render> render = tile->addComponent<Render>().lock();
  render->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture(materialName);

  return tile;
}