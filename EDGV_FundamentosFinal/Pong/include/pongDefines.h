#pragma once

#include <platform/defines.h>

#ifdef _WIN32
  #ifdef PONG_EXPORTS
    #define PONG_API __declspec(dllexport)
  #else
    #define PONG_API __declspec(dllimport)
  #endif
#else
  #define PONG_API __attribute__((visibility("default")))
#endif