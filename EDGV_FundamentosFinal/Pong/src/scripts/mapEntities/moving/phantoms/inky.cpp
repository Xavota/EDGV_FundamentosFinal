#include "scripts/mapEntities/moving/phantoms/inky.h"

#include <tools/textureManager.h>

#include <scene/components/render.h>

void Inky::init(WPtr<GameMap> map, float speed, U8 phantomIndex)
{
  Phantom::init(map, speed, phantomIndex);

  m_pRenderComp->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture("phantoms_inky_l0");
}

void Inky::reset()
{
  Phantom::reset();

  m_pRenderComp->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture("phantoms_inky_l0");
}