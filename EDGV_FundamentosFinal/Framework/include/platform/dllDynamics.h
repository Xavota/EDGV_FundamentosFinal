#pragma once

#include "platform/stdHeaders.h"

/**
 * @brief
 * A function type, for further use in dll function extraction.
 */
using functionType = void(*)();

/**
 * @brief
 * Loads dll from files and makes it easy to extract values from it.
 */
class DLLDynamics
{
 public:
  /**
   * @brief
   * The default constructor.
   */
  DLLDynamics() = default;
  /**
   * @brief
   * Initializes the dll.
   * 
   * @description
   * Initializes the dll from a dll file.
   *
   * @param dllPath
   * The file path of the dll.
   */
  DLLDynamics(const WString& dllPath);

  /**
   * @brief
   * The default destructor.
   */
  ~DLLDynamics() = default;

  /**
   * @brief
   * Initializes the dll.
   * 
   * @description
   * Initializes the dll from a dll file.
   *
   * @param dllPath
   * The file path of the dll.
   * 
   * @return
   * If it succeeded or failed to initialize
   */
  bool
  initialize(const WString& dllPath);

  /**
   * @brief
   * Gets a function from the loaded dll.
   *
   * @description
   * Looks for a value inside the dll and loads it like a function.
   *
   * @param functName
   * The name of the function to look.
   *
   * @return
   * The located function ready to execute, nullptr if not found.
   */
  FunctionPtr<void>
  getFunction(const String& functName);

 private:
  /*
   * The loaded instance of a dll.
   */
  void* m_dllInstance = nullptr;
};