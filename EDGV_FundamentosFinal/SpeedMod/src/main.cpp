#include <iostream>

#ifdef _WIN32
  #ifdef SPEED_MOD_DLL_EXPORTS
    #define SPEED_MOD_DLL_API __declspec(dllexport)
  #else
    #define SPEED_MOD_DLL_API __declspec(dllimport)
  #endif
#else
  #define SPEED_MOD_DLL_API __attribute__((visibility("default")))
#endif

extern "C" SPEED_MOD_DLL_API void initMod()
{
  std::cout << "SpeedMod Loaded" << std::endl;
}