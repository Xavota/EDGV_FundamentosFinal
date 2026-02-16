#include <iostream>

#include "newMapModDefines.h"

//#include "newMapManager.h"

extern "C" NEW_MAP_MOD_DLL_API void initMod()
{
  //MapManager::startUp<NewMapManager>();
  //MapManager::test = 2;

  /*if (MapManager::isStarted()) {
    std::cout << "MapManager Loaded" << std::endl;
  }*/
  std::cout << "NewMapMod Loaded" << std::endl;
}