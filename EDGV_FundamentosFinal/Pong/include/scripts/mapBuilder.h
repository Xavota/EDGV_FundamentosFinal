#pragma once

#include <scene/components/script.h>

class GameMap;

namespace eTILE_TYPE
{
  enum E : I8
  {
    kOut = -1,
    kNone = 0,
    kDoubleWall,
    kSingleWall,
    kPhantomSpawnWall,
    kPhantomSpawnExit,
    kOrb,
    kPowerOrb,
    kPlayerSpawn,
  };
}

class MapBuilder : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  MapBuilder() = default;
  /**
   * @brief Default destructor.
   */
  ~MapBuilder() = default;

  void init(const WString& mapFile, SPtr<GameMap> map);

 private:
  void placeDoubleWall(const sf::Vector2u& coord, Vector<I8> neighbours,
                       SPtr<GameMap> map);
  void placeSingleWall(const sf::Vector2u& coord, Vector<I8> neighbours,
                       SPtr<GameMap> map);
  void placePhantomSpawnWall(const sf::Vector2u& coord, Vector<I8> neighbours,
                             SPtr<GameMap> map);
  void placePhantomSpawnExit(const sf::Vector2u& coord, Vector<I8> neighbours,
                             SPtr<GameMap> map);

  SPtr<Actor> addTile(const sf::Vector2u& coord, const String& materialName,
                      SPtr<GameMap> map);
};