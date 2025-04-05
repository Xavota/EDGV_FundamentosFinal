#include "scripts/mapBuilder.h"

#include <platform/iofile.h>
#include <tools/textureManager.h>
#include <scene/scene.h>
#include <scene/actor.h>
#include <scene/components/transform.h>
#include <scene/components/shapeRect.h>
#include <scene/components/render.h>

#include "scripts/mapEntities/coin.h"
#include "scripts/mapEntities/powerCoin.h"
#include "scripts/gameMap.h"

void MapBuilder::init(const WString& fileName, SPtr<GameMap> map)
{
  File mapFile(fileName, eOPEN_TYPE::kReadOnly);

  U32 width = 0;
  U32 height = 0;

  Vector<String> mapChars;
  while (!mapFile.isAtEnd()) {
    const String line = mapFile.getLine();
    mapChars.push_back(line);

    width = std::max(width, static_cast<U32>(line.size()));
  }
  height = static_cast<U32>(mapChars.size());


  map->m_vMapTiles = Vector<Vector<U8>>(height);

  U32 mapHeight = static_cast<U32>(mapChars.size());
  for (U32 i = 0; i < mapHeight; ++i) {
    map->m_vMapTiles[i] = Vector<U8>(width, 0);
    
    U32 rowWidth = static_cast<U32>(mapChars[i].size());
    for (U32 j = 0; j < rowWidth; ++j) {
      char tile = mapChars[i][j];
      Vector<I8> neigbours =  {
        static_cast<I8>((i == 0               || j == 0              ? '/' :
                        mapChars[i - 1][j - 1]) - '0'),
        static_cast<I8>((i == 0                                      ? '/' :
                        mapChars[i - 1][j    ]) - '0'),
        static_cast<I8>((i == 0               || j == (rowWidth - 1) ? '/' :
                        mapChars[i - 1][j + 1]) - '0'),
        static_cast<I8>((                        j == (rowWidth - 1) ? '/' :
                        mapChars[i    ][j + 1]) - '0'),
        static_cast<I8>((i == (mapHeight - 1) || j == (rowWidth - 1) ? '/' :
                        mapChars[i + 1][j + 1]) - '0'),
        static_cast<I8>((i == (mapHeight - 1)                        ? '/' :
                        mapChars[i + 1][j    ]) - '0'),
        static_cast<I8>((i == (mapHeight - 1) || j == 0              ? '/' :
                        mapChars[i + 1][j - 1]) - '0'),
        static_cast<I8>((                        j == 0              ? '/' :
                        mapChars[i    ][j - 1]) - '0'),
      };

      if (tile == '0') { // Normal path
        map->m_vMapTiles[i][j] = eMAP_TILE_TYPE::kPath;
      }
      else if (tile == '1') { // Double wall
        placeDoubleWall({j, i}, neigbours, map);
      }
      else if (tile == '2') { // Single wall
        placeSingleWall({j, i}, neigbours, map);
      }
      else if (tile == '3') { // Phantom Spawn wall
        placePhantomSpawnWall({j, i}, neigbours, map);
      }
      else if (tile == '4') { // Phantom Spawn exit
        placePhantomSpawnExit({j, i}, neigbours, map);
      }
      else if (tile == '5') { // Coin
        map->m_vMapTiles[i][j] = eMAP_TILE_TYPE::kPath;
        SPtr<Actor> tile = addTile({j, i}, "collectables_coin", map);
        tile->addComponent<Coin>();
      }
      else if (tile == '6') { // Power Coin
        map->m_vMapTiles[i][j] = eMAP_TILE_TYPE::kPath;
        SPtr<Actor> tile = addTile({j, i}, "collectables_powerCoin", map);
        tile->addComponent<PowerCoin>();
      }
      else if (tile == '7') { // Player Spawn
        map->m_vMapTiles[i][j] = eMAP_TILE_TYPE::kPlayerSpawn;
      }
      else {
        map->m_vMapTiles[i][j] = eMAP_TILE_TYPE::kPath;
        addTile({j, i}, "error", map);
      }
    }
  }
}

void MapBuilder::placeDoubleWall(const sf::Vector2u& coord,
                                 Vector<I8> neighbours, SPtr<GameMap> map)
{
  map->m_vMapTiles[coord.y][coord.x] = eMAP_TILE_TYPE::kWall;

  // For this case, a neightbour being outside the map is the same as being a
  // double wall
  bool renderWall = false;
  for (U32 i = 0; i < 8; ++i) {
    if (neighbours[i] == eTILE_TYPE::kOut) {
      neighbours[i] = eTILE_TYPE::kDoubleWall;
    }
    if (neighbours[i] != eTILE_TYPE::kDoubleWall &&
        neighbours[i] != eTILE_TYPE::kSingleWall) {
      renderWall = true;
    }
    //std::cout << I32(neighbours[i]);
  }
  if (!renderWall) {
    return;
  }
  //std::cout << std::endl;


  // up - single wall
  if (neighbours[1] == eTILE_TYPE::kSingleWall) {
    //std::cout << "s";
    // right - wall
    if (neighbours[3] == eTILE_TYPE::kDoubleWall ||
        neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "w";
      // down - any
      {
        //std::cout << "x";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          // up left - empty
          if (neighbours[0] != eTILE_TYPE::kDoubleWall &&
              neighbours[0] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uli";
            //std::cout << std::endl;
            addTile(coord, "walls_double_swxw_uli", map);
            return;
          }
          // up right - empty
          if (neighbours[2] != eTILE_TYPE::kDoubleWall &&
              neighbours[2] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uri";
            //std::cout << std::endl;
            addTile(coord, "walls_double_swxw_uri", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  // up - wall
  if (neighbours[1] == eTILE_TYPE::kDoubleWall ||
      neighbours[1] == eTILE_TYPE::kSingleWall) {
    //std::cout << "w";
    // right - single wall
    if (neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "s";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - any
        {
          //std::cout << "x";
          // up right - empty
          if (neighbours[2] != eTILE_TYPE::kDoubleWall &&
              neighbours[2] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uri";
            //std::cout << std::endl;
            addTile(coord, "walls_double_wswx_uri", map);
            return;
          }
          // down right - empty
          if (neighbours[4] != eTILE_TYPE::kDoubleWall &&
              neighbours[4] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uli";
            //std::cout << std::endl;
            addTile(coord, "walls_double_wswx_dri", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - wall
    if (neighbours[3] == eTILE_TYPE::kDoubleWall ||
        neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "w";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // left - empty
        if (neighbours[7] != eTILE_TYPE::kDoubleWall &&
            neighbours[7] != eTILE_TYPE::kSingleWall) {
          //std::cout << "i";
          //std::cout << std::endl;
          addTile(coord, "walls_double_wwwi", map);
          return;
        }
        //std::cout << ".";
      }
      // down - empty
      if (neighbours[5] != eTILE_TYPE::kDoubleWall &&
          neighbours[5] != eTILE_TYPE::kSingleWall) {
        //std::cout << "i";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_double_wwiw", map);
          return;
        }
        //std::cout << ".";
      }
      // down - any
      {
        //std::cout << "x";
        // down - any
        {
          //std::cout << "x";
          // up right - empty
          if (neighbours[2] != eTILE_TYPE::kDoubleWall &&
              neighbours[2] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uri";
            //std::cout << std::endl;
            addTile(coord, "walls_double_wwxx_uri", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - empty
    if (neighbours[3] != eTILE_TYPE::kDoubleWall &&
        neighbours[3] != eTILE_TYPE::kSingleWall) {
      //std::cout << "i";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_double_wiww", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - any
    {
      //std::cout << "x";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - single wall
        if (neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "s";
          // up left - empty
          if (neighbours[0] != eTILE_TYPE::kDoubleWall &&
              neighbours[0] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uli";
            //std::cout << std::endl;
            addTile(coord, "walls_double_wxws_uli", map);
            return;
          }
          // up right - empty
          if (neighbours[6] != eTILE_TYPE::kDoubleWall &&
              neighbours[6] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_dli";
            //std::cout << std::endl;
            addTile(coord, "walls_double_wxws_dli", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      // down - any
      {
        //std::cout << "x";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          // up left - empty
          if (neighbours[0] != eTILE_TYPE::kDoubleWall &&
              neighbours[0] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uli";
            //std::cout << std::endl;
            addTile(coord, "walls_double_wxxw_uli", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  // up - empty
  if (neighbours[1] != eTILE_TYPE::kDoubleWall &&
      neighbours[1] != eTILE_TYPE::kSingleWall) {
    //std::cout << "i";
    // right - wall
    if (neighbours[3] == eTILE_TYPE::kDoubleWall ||
        neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "w";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_double_iwww", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  // up - any
  {
    //std::cout << "x";
    // right - wall
    if (neighbours[3] == eTILE_TYPE::kDoubleWall ||
        neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "w";
      // down - single wall
      if (neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "s";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          // up left - empty
          if (neighbours[4] != eTILE_TYPE::kDoubleWall &&
              neighbours[4] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_dri";
            //std::cout << std::endl;
            addTile(coord, "walls_double_xwsw_dri", map);
            return;
          }
          // up right - empty
          if (neighbours[6] != eTILE_TYPE::kDoubleWall &&
              neighbours[6] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_dli";
            //std::cout << std::endl;
            addTile(coord, "walls_double_xwsw_dli", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - any
        {
          //std::cout << "x";
          // up right - empty
          if (neighbours[4] != eTILE_TYPE::kDoubleWall &&
              neighbours[4] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_dri";
            //std::cout << std::endl;
            addTile(coord, "walls_double_xwwx_dri", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - any
    {
      //std::cout << "x";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          // up left - empty
          if (neighbours[6] != eTILE_TYPE::kDoubleWall &&
              neighbours[6] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_dli";
            //std::cout << std::endl;
            addTile(coord, "walls_double_xxww_dli", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }

  //std::cout << std::endl;
  addTile(coord, "error", map);
}

void MapBuilder::placeSingleWall(const sf::Vector2u& coord,
                                 Vector<I8> neighbours, SPtr<GameMap> map)
{
  map->m_vMapTiles[coord.y][coord.x] = eMAP_TILE_TYPE::kWall;

  // For this case, a neightbour being outside the map is the same as being a
  // single wall
  bool renderWall = false;
  for (U32 i = 0; i < 8; ++i) {
    if (neighbours[i] == eTILE_TYPE::kOut) {
      neighbours[i] = eTILE_TYPE::kSingleWall;
    }
    if (neighbours[i] != eTILE_TYPE::kDoubleWall &&
        neighbours[i] != eTILE_TYPE::kSingleWall) {
      renderWall = true;
    }
    //std::cout << I32(neighbours[i]);
  }
  if (!renderWall) {
    return;
  }
  //std::cout << std::endl;

  // up - wall
  if (neighbours[1] == eTILE_TYPE::kDoubleWall ||
      neighbours[1] == eTILE_TYPE::kSingleWall) {
    //std::cout << "w";
    // right - wall
    if (neighbours[3] == eTILE_TYPE::kDoubleWall ||
        neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "w";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // left - empty
        if (neighbours[7] != eTILE_TYPE::kDoubleWall &&
            neighbours[7] != eTILE_TYPE::kSingleWall) {
          //std::cout << "i";
          //std::cout << std::endl;
          addTile(coord, "walls_single_wwwi", map);
          return;
        }
        //std::cout << ".";
      }
      // down - empty
      if (neighbours[5] != eTILE_TYPE::kDoubleWall &&
          neighbours[5] != eTILE_TYPE::kSingleWall) {
        //std::cout << "i";
        // right - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_single_wwiw", map);
          return;
        }
        // right - empty
        if (neighbours[7] != eTILE_TYPE::kDoubleWall &&
            neighbours[7] != eTILE_TYPE::kSingleWall) {
          //std::cout << "i";
          //std::cout << std::endl;
          addTile(coord, "walls_single_wwii", map);
          return;
        }
        //std::cout << ".";
      }
      // down - any
      {
        //std::cout << "x";
        // left - any
        {
          //std::cout << "x";
          // up right - empty
          if (neighbours[2] != eTILE_TYPE::kDoubleWall &&
              neighbours[2] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uri";
            //std::cout << std::endl;
            addTile(coord, "walls_single_wwxx_uri", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - empty
    if (neighbours[3] != eTILE_TYPE::kDoubleWall &&
        neighbours[3] != eTILE_TYPE::kSingleWall) {
      //std::cout << "i";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_single_wiww", map);
          return;
        }
        //std::cout << ".";
      }
      // down - empty
      if (neighbours[5] != eTILE_TYPE::kDoubleWall &&
          neighbours[5] != eTILE_TYPE::kSingleWall) {
        //std::cout << "i";
        // left - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_single_wiiw", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - any
    {
      //std::cout << "x";
      // down - any
      {
        //std::cout << "x";
        // down - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          if (neighbours[0] != eTILE_TYPE::kDoubleWall &&
              neighbours[0] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_uli";
            //std::cout << std::endl;
            addTile(coord, "walls_single_wxxw_uli", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  // up - empty
  if (neighbours[1] != eTILE_TYPE::kDoubleWall &&
      neighbours[1] != eTILE_TYPE::kSingleWall) {
    //std::cout << "i";
    // right - wall
    if (neighbours[3] == eTILE_TYPE::kDoubleWall ||
        neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "w";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // right - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_single_iwww", map);
          return;
        }
        // right - empty
        if (neighbours[7] != eTILE_TYPE::kDoubleWall &&
            neighbours[7] != eTILE_TYPE::kSingleWall) {
          //std::cout << "i";
          //std::cout << std::endl;
          addTile(coord, "walls_single_iwwi", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - empty
    if (neighbours[3] != eTILE_TYPE::kDoubleWall &&
        neighbours[3] != eTILE_TYPE::kSingleWall) {
      //std::cout << "i";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // left - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          //std::cout << std::endl;
          addTile(coord, "walls_single_iiww", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  // up - any
  {
    //std::cout << "x";
    // right - wall
    if (neighbours[3] == eTILE_TYPE::kDoubleWall ||
        neighbours[3] == eTILE_TYPE::kSingleWall) {
      //std::cout << "w";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // down - any
        {
          //std::cout << "x";
          if (neighbours[4] != eTILE_TYPE::kDoubleWall &&
              neighbours[4] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_dri";
            //std::cout << std::endl;
            addTile(coord, "walls_single_xwwx_dri", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - any
    {
      //std::cout << "x";
      // down - wall
      if (neighbours[5] == eTILE_TYPE::kDoubleWall ||
          neighbours[5] == eTILE_TYPE::kSingleWall) {
        //std::cout << "w";
        // left - wall
        if (neighbours[7] == eTILE_TYPE::kDoubleWall ||
            neighbours[7] == eTILE_TYPE::kSingleWall) {
          //std::cout << "w";
          // down left - empty
          if (neighbours[6] != eTILE_TYPE::kDoubleWall &&
              neighbours[6] != eTILE_TYPE::kSingleWall) {
            //std::cout << "_dli";
            //std::cout << std::endl;
            addTile(coord, "walls_single_xxww_dli", map);
            return;
          }
          //std::cout << ".";
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }

  //std::cout << "error";
  //std::cout << std::endl;
  addTile(coord, "error", map);
}

void MapBuilder::placePhantomSpawnWall(const sf::Vector2u& coord,
                                       Vector<I8> neighbours, SPtr<GameMap> map)
{
  // For this case, a neightbour being outside the map is the same as being a
  // phantom spawn wall
  bool renderWall = false;
  for (U32 i = 0; i < 8; ++i) {
    if (neighbours[i] == eTILE_TYPE::kOut) {
        neighbours[i] = eTILE_TYPE::kPhantomSpawnWall;
    }
    //std::cout << I32(neighbours[i]);
    if (neighbours[i] != eTILE_TYPE::kPhantomSpawnWall &&
        neighbours[i] != eTILE_TYPE::kPhantomSpawnExit) {
      renderWall = true;
    }
  }
  if (!renderWall) {
    map->m_vMapTiles[coord.y][coord.x] = eMAP_TILE_TYPE::kPhantomSpawn;
    return;
  }
  map->m_vMapTiles[coord.y][coord.x] = eMAP_TILE_TYPE::kWall;
  //std::cout << std::endl;

  // up - spawn
  if (neighbours[1] == eTILE_TYPE::kPhantomSpawnWall ||
      neighbours[1] == eTILE_TYPE::kPhantomSpawnExit) {
    //std::cout << "s";
    // right - spawn
    if (neighbours[3] == eTILE_TYPE::kPhantomSpawnWall ||
        neighbours[3] == eTILE_TYPE::kPhantomSpawnExit) {
      //std::cout << "s";
      // down - spawn
      if (neighbours[5] == eTILE_TYPE::kPhantomSpawnWall ||
          neighbours[5] == eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "s";
        // left - out
        if (neighbours[7] != eTILE_TYPE::kPhantomSpawnWall &&
            neighbours[7] != eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "o";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_ssso", map);
          return;
        }
        //std::cout << ".";
      }
      // down - out
      if (neighbours[5] != eTILE_TYPE::kPhantomSpawnWall &&
          neighbours[5] != eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "o";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_ssos", map);
          return;
        }
        // left - out
        if (neighbours[7] != eTILE_TYPE::kPhantomSpawnWall &&
            neighbours[7] != eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "o";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_ssoo", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - out
    if (neighbours[3] != eTILE_TYPE::kPhantomSpawnWall &&
        neighbours[3] != eTILE_TYPE::kPhantomSpawnExit) {
      //std::cout << "o";
      // down - spawn
      if (neighbours[5] == eTILE_TYPE::kPhantomSpawnWall ||
          neighbours[5] == eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "s";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_soss", map);
          return;
        }
        //std::cout << ".";
      }
      // down - out
      if (neighbours[5] != eTILE_TYPE::kPhantomSpawnWall &&
          neighbours[5] != eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "o";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_soos", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  // up - out
  if (neighbours[1] != eTILE_TYPE::kPhantomSpawnWall &&
      neighbours[1] != eTILE_TYPE::kPhantomSpawnExit) {
    //std::cout << "o";
    // right - spawn
    if (neighbours[3] == eTILE_TYPE::kPhantomSpawnWall ||
        neighbours[3] == eTILE_TYPE::kPhantomSpawnExit) {
      //std::cout << "s";
      // down - spawn
      if (neighbours[5] == eTILE_TYPE::kPhantomSpawnWall ||
          neighbours[5] == eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "s";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_osss", map);
          return;
        }
        // left - out
        if (neighbours[7] != eTILE_TYPE::kPhantomSpawnWall &&
            neighbours[7] != eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "o";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_osso", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - out
    if (neighbours[3] != eTILE_TYPE::kPhantomSpawnWall &&
        neighbours[3] != eTILE_TYPE::kPhantomSpawnExit) {
      //std::cout << "o";
      // down - spawn
      if (neighbours[5] == eTILE_TYPE::kPhantomSpawnWall ||
          neighbours[5] == eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "s";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_ooss", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }

  //std::cout << "error";
  //std::cout << std::endl;
  addTile(coord, "error", map);
}

void MapBuilder::placePhantomSpawnExit(const sf::Vector2u& coord,
                                       Vector<I8> neighbours, SPtr<GameMap> map)
{
  map->m_vMapTiles[coord.y][coord.x] = eMAP_TILE_TYPE::kPhantomSpawnExit;

  // For this case, a neightbour being outside the map is the same as being a
  // phantom spawn wall
  for (U32 i = 0; i < 8; ++i) {
    if (neighbours[i] == eTILE_TYPE::kOut) {
      neighbours[i] = eTILE_TYPE::kPhantomSpawnWall;
    }
    //std::cout << I32(neighbours[i]);
  }
  //std::cout << std::endl;

  // up - spawn
  if (neighbours[1] == eTILE_TYPE::kPhantomSpawnWall ||
      neighbours[1] == eTILE_TYPE::kPhantomSpawnExit) {
    //std::cout << "s";
    // right - spawn
    if (neighbours[3] == eTILE_TYPE::kPhantomSpawnWall ||
        neighbours[3] == eTILE_TYPE::kPhantomSpawnExit) {
      //std::cout << "s";
      // down - spawn
      if (neighbours[5] == eTILE_TYPE::kPhantomSpawnWall ||
          neighbours[5] == eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "s";
        // left - out
        if (neighbours[7] != eTILE_TYPE::kPhantomSpawnWall &&
            neighbours[7] != eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "o";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_ssso_e", map);
          return;
        }
        //std::cout << ".";
      }
      // down - out
      if (neighbours[5] != eTILE_TYPE::kPhantomSpawnWall &&
          neighbours[5] != eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "o";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_ssos_e", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    // right - out
    if (neighbours[3] != eTILE_TYPE::kPhantomSpawnWall &&
        neighbours[3] != eTILE_TYPE::kPhantomSpawnExit) {
      //std::cout << "o";
      // down - spawn
      if (neighbours[5] == eTILE_TYPE::kPhantomSpawnWall ||
          neighbours[5] == eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "s";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_soss_e", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  // up - out
  if (neighbours[1] != eTILE_TYPE::kPhantomSpawnWall &&
      neighbours[1] != eTILE_TYPE::kPhantomSpawnExit) {
    //std::cout << "o";
    // right - spawn
    if (neighbours[3] == eTILE_TYPE::kPhantomSpawnWall ||
        neighbours[3] == eTILE_TYPE::kPhantomSpawnExit) {
      //std::cout << "s";
      // down - spawn
      if (neighbours[5] == eTILE_TYPE::kPhantomSpawnWall ||
          neighbours[5] == eTILE_TYPE::kPhantomSpawnExit) {
        //std::cout << "s";
        // left - spawn
        if (neighbours[7] == eTILE_TYPE::kPhantomSpawnWall ||
            neighbours[7] == eTILE_TYPE::kPhantomSpawnExit) {
          //std::cout << "s";
          //std::cout << std::endl;
          addTile(coord, "walls_spawn_osss_e", map);
          return;
        }
        //std::cout << ".";
      }
      //std::cout << ".";
    }
    //std::cout << ".";
  }
  
  //std::cout << "error";
  //std::cout << std::endl;
  addTile(coord, "error", map);
}

SPtr<Actor> MapBuilder::addTile(const sf::Vector2u& coord,
                                const String& materialName, SPtr<GameMap> map)
{
  String tileName =
   "t(" + std::to_string(coord.x) + "," + std::to_string(coord.y) + ")";
  SPtr<Actor> tile = getScene().lock()->addActor(tileName).lock();

  //std::cout << tileName << " " << materialName << std::endl;

  SPtr<Transform> transform = tile->getTransform().lock();
  transform->attachTo(map->getTransform());

  transform->setLocalPosition(static_cast<const sf::Vector2f>(coord));
  tile->addComponent<ShapeRect>();
  SPtr<Render> render = tile->addComponent<Render>().lock();
  render->m_material.m_pTexture =
   gl::TextureManager::instance().getTexture(materialName);

  return tile;
}