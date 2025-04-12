#pragma once

#include <scripts/mapManager.h>

class NewMapManager : public MapManager
{
 public:
  void onStartUp() override
  {
    m_sMapFile = L"../mods/NewMapMod/resources/newMap.map";
    std::cout << "Started up NewMapManager" << std::endl;
  }
};