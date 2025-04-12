#include <iostream>

#include "mapManager.h"

#ifdef _WIN32
  #ifdef NEW_MAP_MOD_DLL_EXPORTS
    #define NEW_MAP_MOD_DLL_API __declspec(dllexport)
  #else
    #define NEW_MAP_MOD_DLL_API __declspec(dllimport)
  #endif
#else
  #define NEW_MAP_MOD_DLL_API __attribute__((visibility("default")))
#endif

extern "C" NEW_MAP_MOD_DLL_API void initMod()
{
  MapManager::startUp<NewMapManager>();

  if (MapManager::isStarted()) {
    std::cout << "NewMapMod Loaded" << std::endl;
  }
}