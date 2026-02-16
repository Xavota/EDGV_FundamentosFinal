#pragma once

//#include <platform/module.hpp>

#include <platform/stdHeaders.h>

#include "pongDefines.h"

class PONG_API MapManager// : public Module<MapManager>
{
 public:
  MapManager() = default;
  ~MapManager() = default;

  WString m_sMapFile = L"../Pong/resources/baseMap.map";

  //static int test;
};