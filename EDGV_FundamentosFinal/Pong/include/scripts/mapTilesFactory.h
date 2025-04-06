#pragma once

#include <scene/components/script.h>

class Actor;
class Scene;
class Transform;

class MapTilesFactory
{
 public:
  static SPtr<Actor> factory(WPtr<Scene> scene, const sf::Vector2u& coord,
                             const String& materialName, WPtr<Transform> parent);
};
