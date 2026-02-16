#pragma once

#include <scripts/mapManager.h>

#include "newMapModDefines.h"

class NEW_MAP_MOD_DLL_API NewMapManager : public MapManager
{
 public:
  NewMapManager() = default;
  ~NewMapManager() = default;
 
  /*void onStartUp() override
  {
    m_sMapFile = L"../mods/NewMapMod/resources/newMap.map";
    std::cout << "Started up NewMapManager" << std::endl;
  }*/
};