#include <frameworkManager.h>

int main() {
  FrameworkManager::startUp();
  FrameworkManager::instance().run({1024, 1024}, "Pong", true);

  return 0;
}