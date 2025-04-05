#include "scripts/mapEntities/moving/phantoms/blinky.h"

#include <tools/textureManager.h>

#include <scene/components/render.h>

void Blinky::init(WPtr<GameMap> map, float speed, U8 phantomIndex)
{
  Phantom::init(map, speed, phantomIndex);

  m_pRenderComp->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture("phantoms_blinky_l0");
}