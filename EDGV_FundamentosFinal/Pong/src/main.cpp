#include <frameworkManager.h>

#include "pongFramework.h"

int main() {
  FrameworkManager::startUp<PongFramework>();
  FrameworkManager::instance().run({1024, 1024}, "Pong", true);

  return 0;
}