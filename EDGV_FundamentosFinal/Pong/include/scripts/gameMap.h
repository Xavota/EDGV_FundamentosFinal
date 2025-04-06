#pragma once

#include <scene/components/script.h>

class Collectable;
class File;

namespace eMAP_TILE_TYPE
{
  enum E : U8
  {
    kPath,
    kWall,
    kPhantomSpawn,
    kPhantomSpawnExit,
    kWrappingPath,
    kWrappingEntrance,
    kPlayerSpawn,
  };
}

namespace eMOVEMENT_DIR
{
  enum E : U8
  {
    kUp = 1,
    kRight = 1 << 1,
    kDown = 1 << 2,
    kLeft = 1 << 3,
  };
}

class GameMap : public Script
{
 public:
  /**
   * @brief Default constructor.
   */
  GameMap() = default;
  /**
   * @brief Default destructor.
   */
  virtual ~GameMap() = default;
  

  struct SpawnInfo
  {
    sf::Vector2u mapPos;
    sf::Vector2i initialDir;
    sf::Vector2f posOffset;
  };

  struct WrapInfo
  {
    sf::Vector2u entrancePos;
    sf::Vector2u exitPos;
    sf::Vector2i entranceDir;
    sf::Vector2i exitDir;
  };


  virtual void saveToFile(File& saveFile);
  virtual void loadFromFile(File& loadFile);

  bool init(FuntionPtr<void> collectedAll);
  void restart();

  virtual bool neighbourTileIsOfType(const sf::Vector2u origin, U8 dir, U8 type) const;

  virtual U8 getMovementOptions(const sf::Vector2u& coord) const;
  virtual bool isPath(const sf::Vector2i& coord) const;
  virtual bool isWrapping(const sf::Vector2u coord, WrapInfo& outInfo) const;

  virtual SpawnInfo getPlayerSpawnInfo() const;
  virtual SpawnInfo getPhantomSpawnInfo(U32 index) const;

  virtual void collected(U32 tileCode);


  const float m_fMaxWrappingDistance = 3.0f;

 private:
  friend class MapBuilder;
  friend class GameManager;

  bool m_bIsValid = false;

  void setMapTiles(const Vector<Vector<U8>>& tiles);

  Vector<Vector<U8>> m_vMapTiles;
  Map<U32, SPtr<Collectable>> m_mCollectables;
  U32 m_iActiveCollectables = 0;

  FuntionPtr<void> m_fpCollectedAll = nullptr;

  sf::Vector2u m_wrapUpPosition = {};
  sf::Vector2u m_wrapDownPosition = {};
  sf::Vector2u m_wrapLeftPosition = {};
  sf::Vector2u m_wrapRightPosition = {};

  sf::Vector2u m_phantomSpawnExit1 = {};
  sf::Vector2u m_phantomSpawnExit2 = {};
  U8 m_iPhantomSpawnExitDirection = 0;

  sf::Vector2u m_phantomSpawnStartPosition = {};
  sf::Vector2u m_phantomSpawnEndPosition = {};

  sf::Vector2u m_playerSpawn1 = {};
  sf::Vector2u m_playerSpawn2 = {};
};