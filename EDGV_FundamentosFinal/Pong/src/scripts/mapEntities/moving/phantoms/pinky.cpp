#include "scripts/mapEntities/moving/phantoms/pinky.h"

#include <tools/textureManager.h>

#include <scene/components/render.h>

void Pinky::init(WPtr<GameMap> map, float speed, U8 phantomIndex)
{
  Phantom::init(map, speed, phantomIndex);

  m_pRenderComp->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture("phantoms_pinky_l0");
}

void Pinky::reset()
{
  Phantom::reset();

  m_pRenderComp->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture("phantoms_pinky_l0");
}