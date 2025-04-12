#include "scripts/gameMap.h"

#include <platform/math.h>
#include <platform/iofile.h>

#include <scene/actor.h>

#include "scripts/mapEntities/collectable.h"

void GameMap::saveToFile(File& saveFile)
{
  U32 collectablesCount = static_cast<U32>(m_mCollectables.size());
  saveFile.writeBytes(reinterpret_cast<Byte*>(&collectablesCount), sizeof(U32));
  for (auto& coll : m_mCollectables) {
    U32 tileCode = coll.first;
    bool active = coll.second->getActor().lock()->isLocalActive();
    saveFile.writeBytes(reinterpret_cast<Byte*>(&tileCode), sizeof(U32));
    saveFile.writeBytes(reinterpret_cast<Byte*>(&active), sizeof(bool));
  }
  saveFile.writeBytes(reinterpret_cast<Byte*>(&m_iActiveCollectables), sizeof(U32));

  // TODO: GUARDAR EL RESTO DE COSAS DEL MAPA PARA EVENTUALMENTE ACEPTAR MÁS
  //       ARCHIVOS DE MAPAS
}

void GameMap::loadFromFile(File& loadFile)
{
  U32 collectablesCount = 0;
  loadFile.readBytes(reinterpret_cast<Byte*>(&collectablesCount), sizeof(U32));
  for (U32 i = 0; i < collectablesCount; ++i) {
    U32 tileCode = 0;
    bool active = false;
    loadFile.readBytes(reinterpret_cast<Byte*>(&tileCode), sizeof(U32));
    loadFile.readBytes(reinterpret_cast<Byte*>(&active), sizeof(bool));

    if (!m_mCollectables.contains(tileCode)) continue;
    m_mCollectables[tileCode]->getActor().lock()->setActive(active);
  }
  loadFile.readBytes(reinterpret_cast<Byte*>(&m_iActiveCollectables), sizeof(U32));

  // TODO: GUARDAR EL RESTO DE COSAS DEL MAPA PARA EVENTUALMENTE ACEPTAR MÁS
  //       ARCHIVOS DE MAPAS
}

bool GameMap::init(FunctionPtr<void> collectedAll)
{
  m_bIsValid = true;

  if (m_vMapTiles.empty() || m_vMapTiles[0].empty()) {
    std::cout << "Map tiles shouldn't be empty. Invalid map." << std::endl;
    m_bIsValid = false;
    return false;
  }

  U32 width = static_cast<U32>(m_vMapTiles[0].size());
  U32 height = static_cast<U32>(m_vMapTiles.size());


  m_iActiveCollectables = m_mCollectables.size();

  m_fpCollectedAll = collectedAll;


  bool hasWrapUp = false;
  bool hasWrapDown = false;
  bool hasWrapLeft = false;
  bool hasWrapRight = false;

  for (I32 i = 0; i < width; ++i) {
    // Wrap up
    if (m_vMapTiles[0][i] == eMAP_TILE_TYPE::kPath) {
      if (hasWrapUp) {
        std::cout << "Can't have more than one wrap points in the same side."
                     " Invalid map." << std::endl;
        m_bIsValid = false;
        return false;
      }
      hasWrapUp = true;
      m_wrapUpPosition = {static_cast<U32>(i), 0};

      m_vMapTiles[0][i] = eMAP_TILE_TYPE::kWrappingEntrance;
      for (I32 j = 1; j <= 4; ++j) {
        if (j >= height || !isPath({i, j})) break;

        U8 options = getMovementOptions({static_cast<U32>(i), static_cast<U32>(j)});
        if (Math::hasFlag(options, eMOVEMENT_DIR::kLeft) ||
            Math::hasFlag(options, eMOVEMENT_DIR::kRight)) break;

        m_vMapTiles[j][i] = eMAP_TILE_TYPE::kWrappingPath;
      }
    }
    // Wrap down
    if (m_vMapTiles[height - 1][i] == eMAP_TILE_TYPE::kPath) {
      if (hasWrapDown) {
        std::cout << "Can't have more than one wrap points in the same side. Invalid map." << std::endl;
        m_bIsValid = false;
        return false;
      }
      hasWrapDown = true;
      m_wrapDownPosition = {static_cast<U32>(i), height - 1};

      m_vMapTiles[height - 1][i] = eMAP_TILE_TYPE::kWrappingEntrance;
      for (I32 j = 1; j <= 4; ++j) {
        if (height - 1 - j <= 0 || !isPath({i, static_cast<I32>(height) - 1 - j})) break;

        U8 options = getMovementOptions({static_cast<U32>(i), height - 1 - static_cast<U32>(j)});
        if (Math::hasFlag(options, eMOVEMENT_DIR::kLeft) ||
            Math::hasFlag(options, eMOVEMENT_DIR::kRight)) break;

        m_vMapTiles[height - 1 - j][i] = eMAP_TILE_TYPE::kWrappingPath;
      }
    }
  }

  for (I32 i = 0; i < height; ++i) {
    // Wrap left
    if (m_vMapTiles[i][0] == eMAP_TILE_TYPE::kPath) {
      if (hasWrapLeft) {
        std::cout << "Can't have more than one wrap points in the same side. Invalid map." << std::endl;
        m_bIsValid = false;
        return false;
      }
      hasWrapLeft = true;
      m_wrapLeftPosition = {0, static_cast<U32>(i)};
      std::cout << "m_wrapLeftPosition (" << m_wrapLeftPosition.x << "," << m_wrapLeftPosition.y << ")" << std::endl;

      m_vMapTiles[i][0] = eMAP_TILE_TYPE::kWrappingEntrance;
      for (I32 j = 1; j <= 4; ++j) {
        if (j >= width || !isPath({j, i})) break;

        U8 options = getMovementOptions({static_cast<U32>(j), static_cast<U32>(i)});
        if (Math::hasFlag(options, eMOVEMENT_DIR::kUp) ||
            Math::hasFlag(options, eMOVEMENT_DIR::kDown)) break;

        m_vMapTiles[i][j] = eMAP_TILE_TYPE::kWrappingPath;
      }
    }
    // Wrap right
    if (m_vMapTiles[i][width - 1] == eMAP_TILE_TYPE::kPath) {
      if (hasWrapRight) {
        std::cout << "Can't have more than one wrap points in the same side. Invalid map." << std::endl;
        m_bIsValid = false;
        return false;
      }
      hasWrapRight = true;
      m_wrapRightPosition = {width - 1, static_cast<U32>(i)};
      std::cout << "m_wrapRightPosition (" << m_wrapRightPosition.x << "," << m_wrapRightPosition.y << ")" << std::endl;

      m_vMapTiles[i][width - 1] = eMAP_TILE_TYPE::kWrappingEntrance;
      for (I32 j = 1; j <= 4; ++j) {
        if (width - 1 - j <= 0 || !isPath({static_cast<I32>(width) - 1 - j, i})) break;

        U8 options = getMovementOptions({width - 1 - static_cast<U32>(j), static_cast<U32>(i)});
        if (Math::hasFlag(options, eMOVEMENT_DIR::kUp) ||
            Math::hasFlag(options, eMOVEMENT_DIR::kDown)) break;

        m_vMapTiles[i][width - 1 - j] = eMAP_TILE_TYPE::kWrappingPath;
      }
    }
  }

  bool startedSpawn = false;
  bool startedRow = false;
  bool endedRow = false;
  U32 rightCoord = 0;

  U32 spawnDoorCount = 0;
  U32 spawnPlayerCount = 0;

  String phantomWallErrorMsg = "Phantom Spawn should be a regular rectangle."
                               " Invalid map.";
  String phantomExitErrorMsg = "Phantom Spawn exit should be exactly two"
                               " connected tiles and both tiles should be beside"
                               " one spawn tile on one side and one path tile on"
                               " the opposite. Invalid map.";
  String playerSpawnErrorMsg = "Player spawn should be exactly two connected"
                               " tiles. Invalid map.";

  for (U32 i = 0; i < height; ++i) {
    startedRow = false;
    endedRow = false;
    for (U32 j = 0; j < width; ++j) {
      if (m_vMapTiles[i][j] == eMAP_TILE_TYPE::kPhantomSpawn) {
        if (endedRow) {
          std::cout << phantomWallErrorMsg << std::endl;
          m_bIsValid = false;
          return false;
        }

        if (!startedSpawn) {
          startedSpawn = true;
          m_phantomSpawnStartPosition = {j, i};
        }
        if (!startedRow) {
          startedRow = true;
        }

        if (m_phantomSpawnStartPosition.y != i &&
           (j < m_phantomSpawnStartPosition.x || j > rightCoord)) {
          std::cout << phantomWallErrorMsg << std::endl;
          m_bIsValid = false;
          return false;
        }

        m_phantomSpawnEndPosition = {j, i};
      }
      else if (m_vMapTiles[i][j] == eMAP_TILE_TYPE::kPhantomSpawnExit) {
        ++spawnDoorCount;
        if (spawnDoorCount == 3) {
          std::cout << phantomExitErrorMsg << std::endl;
          m_bIsValid = false;
          return false;
        }

        if (spawnDoorCount == 1) {
          m_phantomSpawnExit1 = {j, i};

          if (neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kUp, eMAP_TILE_TYPE::kPhantomSpawn)) {
            if (!neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kRight, eMAP_TILE_TYPE::kPhantomSpawnExit) ||
                !neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kUp | eMOVEMENT_DIR::kRight, eMAP_TILE_TYPE::kPhantomSpawn) ||
                !isPath({static_cast<I32>(j), static_cast<I32>(i) + 1}) || !isPath({static_cast<I32>(j) + 1, static_cast<I32>(i) + 1})) {
              std::cout << phantomExitErrorMsg << std::endl;
              m_bIsValid = false;
              return false;
            }
            
            m_phantomSpawnExit2 = {j + 1, i};
            m_iPhantomSpawnExitDirection = eMOVEMENT_DIR::kDown;
          }
          else if (neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kRight, eMAP_TILE_TYPE::kPhantomSpawn)) {
            if (!neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kDown, eMAP_TILE_TYPE::kPhantomSpawnExit) ||
                !neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kDown | eMOVEMENT_DIR::kRight, eMAP_TILE_TYPE::kPhantomSpawn) ||
                !isPath({static_cast<I32>(j) - 1, static_cast<I32>(i)}) || !isPath({static_cast<I32>(j) - 1, static_cast<I32>(i) + 1})) {
              std::cout << phantomExitErrorMsg << std::endl;
              m_bIsValid = false;
              return false;
            }
            
            m_phantomSpawnExit2 = {j, i + 1};
            m_iPhantomSpawnExitDirection = eMOVEMENT_DIR::kLeft;
          }
          else if (neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kDown, eMAP_TILE_TYPE::kPhantomSpawn)) {
            if (!neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kRight, eMAP_TILE_TYPE::kPhantomSpawnExit) ||
                !neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kDown | eMOVEMENT_DIR::kRight, eMAP_TILE_TYPE::kPhantomSpawn) ||
                !isPath({static_cast<I32>(j), static_cast<I32>(i) - 1}) || !isPath({static_cast<I32>(j) + 1, static_cast<I32>(i) - 1})) {
              std::cout << phantomExitErrorMsg << std::endl;
              m_bIsValid = false;
              return false;
            }
            
            m_phantomSpawnExit2 = {j + 1, i};
            m_iPhantomSpawnExitDirection = eMOVEMENT_DIR::kUp;
          }
          else if (neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kLeft, eMAP_TILE_TYPE::kPhantomSpawn)) {
            if (!neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kDown, eMAP_TILE_TYPE::kPhantomSpawnExit) ||
                !neighbourTileIsOfType({j, i}, eMOVEMENT_DIR::kDown | eMOVEMENT_DIR::kLeft, eMAP_TILE_TYPE::kPhantomSpawn) ||
                !isPath({static_cast<I32>(j) + 1, static_cast<I32>(i)}) || !isPath({static_cast<I32>(j) + 1, static_cast<I32>(i) + 1})) {
              std::cout << phantomExitErrorMsg << std::endl;
              m_bIsValid = false;
              return false;
            }
            
            m_phantomSpawnExit2 = {j, i + 1};
            m_iPhantomSpawnExitDirection = eMOVEMENT_DIR::kRight;
          }
          else {
            std::cout << phantomExitErrorMsg << std::endl;
            m_bIsValid = false;
            return false;
          }
        }
      }
      else {
        if (startedRow) {
          endedRow = true;

          if (m_phantomSpawnStartPosition.y == i) {
            rightCoord = j;
          }
        }
      }

      if (m_vMapTiles[i][j] == eMAP_TILE_TYPE::kPlayerSpawn) {
        ++spawnPlayerCount;
        if (spawnPlayerCount == 3) {
          std::cout << playerSpawnErrorMsg << std::endl;
          m_bIsValid = false;
          return false;
        }

        if (spawnPlayerCount == 1) {
          m_playerSpawn1 = {j, i};

          if (i > 0 && m_vMapTiles[i - 1][j] == eMAP_TILE_TYPE::kPlayerSpawn) {
            m_playerSpawn2 = {j, i - 1};
          }
          else if (j < (width - 1) && m_vMapTiles[i][j + 1] == eMAP_TILE_TYPE::kPlayerSpawn) {
            m_playerSpawn2 = {j + 1, i};
          }
          else if (i < (height - 1) && m_vMapTiles[i + 1][j] == eMAP_TILE_TYPE::kPlayerSpawn) {
            m_playerSpawn2 = {j, i + 1};
          }
          else if (j > 0 && m_vMapTiles[i][j - 1] == eMAP_TILE_TYPE::kPlayerSpawn) {
            m_playerSpawn2 = {j - 1, i};
          }
          else {
            std::cout << playerSpawnErrorMsg << std::endl;
            m_bIsValid = false;
            return false;
          }
        }
      }
    }
  }

  return true;
}

void GameMap::restart()
{
  for (auto& coll : m_mCollectables) {
    coll.second->getActor().lock()->setActive(true);
  }
  m_iActiveCollectables = m_mCollectables.size();
}

bool GameMap::neighbourTileIsOfType(const sf::Vector2u origin, U8 dir, U8 type) const
{
  U32 maxWidth = static_cast<U32>(m_vMapTiles[origin.y].size()) - 1;
  U32 maxHeight = static_cast<U32>(m_vMapTiles.size()) - 1;

  bool isUp = Math::hasFlag(dir, eMOVEMENT_DIR::kUp);
  bool isRight = Math::hasFlag(dir, eMOVEMENT_DIR::kRight);
  bool isDown = Math::hasFlag(dir, eMOVEMENT_DIR::kDown);
  bool isLeft = Math::hasFlag(dir, eMOVEMENT_DIR::kLeft);

  if (isUp && origin.y == 0) return false;
  if (isRight && origin.x >= maxWidth) return false;
  if (isDown && origin.y >= maxHeight) return false;
  if (isLeft && origin.x == 0) return false;

  sf::Vector2u newDir = origin;
  newDir += sf::Vector2u(0, isUp ? -1 : 0);
  newDir += sf::Vector2u(isRight ? 1 : 0, 0);
  newDir += sf::Vector2u(0, isDown ? 1 : 0);
  newDir += sf::Vector2u(isLeft ? -1 : 0, 0);

  return m_vMapTiles[newDir.y][newDir.x] == type;
}

U8 GameMap::getMovementOptions(const sf::Vector2u& coord) const
{
  U32 maxWidth = static_cast<U32>(m_vMapTiles[coord.y].size()) - 1;
  U32 maxHeight = static_cast<U32>(m_vMapTiles.size()) - 1;

  U8 options = 0;
  options |= (coord.y > 0 && isPath({static_cast<I32>(coord.x), static_cast<I32>(coord.y) - 1})) ?
             eMOVEMENT_DIR::kUp : 0;
  options |= (coord.x < maxWidth && isPath({static_cast<I32>(coord.x) + 1, static_cast<I32>(coord.y)})) ?
             eMOVEMENT_DIR::kRight : 0;
  options |= (coord.y < maxHeight && isPath({static_cast<I32>(coord.x), static_cast<I32>(coord.y) + 1})) ?
             eMOVEMENT_DIR::kDown : 0;
  options |= (coord.x > 0 && isPath({static_cast<I32>(coord.x) - 1, static_cast<I32>(coord.y)})) ?
             eMOVEMENT_DIR::kLeft : 0;

  return options;
}

bool GameMap::isPath(const sf::Vector2i& coord) const
{
  if (coord.x < 0 || coord.x >= m_vMapTiles[coord.y].size() ||
      coord.y < 0 || coord.y >= m_vMapTiles.size()) return false;
  
  I8 tileType = m_vMapTiles[coord.y][coord.x];
  return tileType == eMAP_TILE_TYPE::kPath ||
         tileType == eMAP_TILE_TYPE::kWrappingPath ||
         tileType == eMAP_TILE_TYPE::kWrappingEntrance ||
         tileType == eMAP_TILE_TYPE::kPlayerSpawn;
}

bool GameMap::isWrapping(const sf::Vector2u coord, WrapInfo& outInfo) const
{
  if (coord == m_wrapUpPosition) {
    outInfo.entrancePos = m_wrapUpPosition;
    outInfo.exitPos = m_wrapDownPosition;
    outInfo.entranceDir = {0, -1};
    outInfo.exitDir = {0, -1};
    return true;
  }
  if (coord == m_wrapRightPosition) {
    outInfo.entrancePos = m_wrapRightPosition;
    outInfo.exitPos = m_wrapLeftPosition;
    outInfo.entranceDir = {1, 0};
    outInfo.exitDir = {1, 0};
    return true;
  }
  if (coord == m_wrapDownPosition) {
    outInfo.entrancePos = m_wrapDownPosition;
    outInfo.exitPos = m_wrapUpPosition;
    outInfo.entranceDir = {0, 1};
    outInfo.exitDir = {0, 1};
    return true;
  }
  if (coord == m_wrapLeftPosition) {
    outInfo.entrancePos = m_wrapLeftPosition;
    outInfo.exitPos = m_wrapRightPosition;
    outInfo.entranceDir = {-1, 0};
    outInfo.exitDir = {-1, 0};
    return true;
  }
  return false;
}

void GameMap::setMapTiles(const Vector<Vector<U8>>& tiles)
{
  m_vMapTiles = tiles;
}

GameMap::SpawnInfo GameMap::getPlayerSpawnInfo() const
{
  if (!m_bIsValid) return {};

  SpawnInfo info = {};
  info.mapPos = m_playerSpawn2;
  info.initialDir = sf::Vector2i(
    static_cast<I32>(m_playerSpawn1.x) - m_playerSpawn2.x,
    static_cast<I32>(m_playerSpawn1.y) - m_playerSpawn2.y);
  info.posOffset = sf::Vector2f(
    info.initialDir.x / 2.0f,
    info.initialDir.y / 2.0f);

  return info;
}

GameMap::SpawnInfo GameMap::getPhantomSpawnInfo(U32 index) const
{
  if (!m_bIsValid) return {};

  SpawnInfo info = {};
  if (index == 0) {
    sf::Vector2i spawnDir = {};
    if (Math::hasFlag(m_iPhantomSpawnExitDirection, eMOVEMENT_DIR::kUp)) {
      spawnDir = {0, -1};
    }
    else if (Math::hasFlag(m_iPhantomSpawnExitDirection, eMOVEMENT_DIR::kRight)) {
      spawnDir = {1, 0};
    }
    else if (Math::hasFlag(m_iPhantomSpawnExitDirection, eMOVEMENT_DIR::kDown)) {
      spawnDir = {0, 1};
    }
    else if (Math::hasFlag(m_iPhantomSpawnExitDirection, eMOVEMENT_DIR::kLeft)) {
      spawnDir = {-1, 0};
    }

    info.mapPos = m_phantomSpawnExit2 + static_cast<sf::Vector2u>(spawnDir);
    info.initialDir = sf::Vector2i(
      static_cast<I32>(m_phantomSpawnExit1.x) - m_phantomSpawnExit2.x,
      static_cast<I32>(m_phantomSpawnExit1.y) - m_phantomSpawnExit2.y);
    info.posOffset = sf::Vector2f(
      info.initialDir.x / 2.0f,
      info.initialDir.y / 2.0f);
  }


  return info;
}

void GameMap::collected(U32 tileCode)
{
  if (!m_mCollectables.contains(tileCode)) return;

  m_mCollectables[tileCode]->getActor().lock()->setActive(false);
  --m_iActiveCollectables;

  std::cout << "Collected. " << m_iActiveCollectables << " left" << std::endl;

  if (m_iActiveCollectables <= 0 && m_fpCollectedAll != nullptr) {
    m_fpCollectedAll();
  }
}