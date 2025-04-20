#include "platform/dllDynamics.h"
#include "platform/defines.h"

#if PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

DLLDynamics::DLLDynamics(const WString& dllPath)
{
  if (!initialize(dllPath)) {
    m_dllInstance = nullptr;
  }
}
bool
DLLDynamics::initialize(const WString& dllPath)
{
#if PLATFORM == PLATFORM_WIN32
  m_dllInstance = reinterpret_cast<void*>(LoadLibrary(dllPath.c_str()));

  if (!m_dllInstance) {
    std::cout << "Could not load DLL" << std::endl;
    return false;
  }

#else
  String narrowDllPath(dllPath.begin(), dllPath.end());
  void* hGetProcIdDLL = dlopen(narrowDllPath.c_str(), RTLD_LAZY);
  if (!hGetProcIdDLL) {
    std::cout << "Could not load DLL" << std::endl;
    return 1;
  }

  m_dllInstance = hGetProcIdDLL;
#endif
  return true;
}
FunctionPtr<void>
DLLDynamics::getFunction(const String& functName)
{
#if PLATFORM == PLATFORM_WIN32
  auto hGetProcIdDLL = reinterpret_cast<HMODULE>(m_dllInstance);
  auto function =
  reinterpret_cast<functionType>(GetProcAddress(hGetProcIdDLL,
                                                functName.c_str()));

  if (!function) {
    std::cout << "Could not load function" << std::endl;
    return nullptr;
  }
#else
  functionType function = (functionType)dlsym(m_dllInstance, "initPlugin");
  if (!function) {
    std::cout << "Could not load function" << std::endl;
    return nullptr;
  }
#endif
  return function;
}