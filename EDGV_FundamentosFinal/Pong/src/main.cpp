#include <frameworkManager.h>

#include "pongFramework.h"

int main() {
  FrameworkManager::startUp<PongFramework>();
  FrameworkManager::instance().run({672, 888}, "Pong", true);

  return 0;
}