#pragma once

#include <platform/module.hpp>

class MapManager : public Module<MapManager>
{
 public:
  WString m_sMapFile = L"../Pong/resources/baseMap.map";
};