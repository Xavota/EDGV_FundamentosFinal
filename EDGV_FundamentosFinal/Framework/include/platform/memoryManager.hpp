#pragma once

#include "platform/defines.h"
#include "platform/stdHeaders.h"

/**
 * @brief
 * For managing the memory allocation and release.
 */
class MemoryManager
{
 public:
  /**
   * @brief
   * The default constructor.
   */
  MemoryManager() = default;
  /**
   * @brief
   * The default destructor.
   */
  ~MemoryManager() = default;

  /**
   * @brief   Creates a new unique pointer of the template class.
   * @param     _Args: The arguments of the constructor for the object.
   * @return  The instance of the new SPtr.
   */
  template <typename T, typename... Args>
  static FORCEINLINE UPtr<T> createUnique(Args&&... _Args);
  /**
   * @brief   Creates a new shared pointer of the template class.
   * @param     _Args: The arguments of the constructor for the object.
   * @return  The instance of the new SPtr.
   */
    template <typename T, typename... Args>
    static FORCEINLINE SPtr<T> createShared(Args&&... _Args);

  /**
   * @brief   Static casts a SPtr changing it for other type of class,
   *          indicated in the template class.
   * @param     ptr The SPtr to reinterpret.
   * @return  The instance of a SPtr reinterpreted.
   */
  template<typename T, typename U>
  static FORCEINLINE SPtr<T> sharedStaticCast(const SPtr<U>& ptr);
  /**
   * @brief   Dyanamicly casts a SPtr changing it for other type of class,
   *          indicated in the template class.
   * @param     ptr The SPtr to reinterpret.
   * @return  The instance of a SPtr reinterpreted.
   */
  template<typename T, typename U>
  static FORCEINLINE SPtr<T> sharedDynamicCast(const SPtr<U>& ptr);
  /**
   * @brief   Const casts a SPtr changing it for other type of class,
   *          indicated in the template class.
   * @param     ptr The SPtr to reinterpret.
   * @return  The instance of a SPtr reinterpreted.
   */
  template<typename T, typename U>
  static FORCEINLINE SPtr<T> sharedConstCast(const SPtr<U>& ptr);
  /**
   * @brief   Reinterprets a SPtr changing it for other type of class,
   *          indicated in the template class.
   * @param     ptr The SPtr to reinterpret.
   * @return  The instance of a SPtr reinterpreted.
   */
  template<typename T, typename U>
  static FORCEINLINE SPtr<T> sharedReinterpretCast(const SPtr<U>& ptr);

  /**
   * @brief   Reinterprets a UPtr changing it for other type of class,
   *          indicated in the template class.
   *          This cast transfers the ownership of the given pointer, and
   *          releases it, so be careful when you do this.
   * @param     ptr The UPtr to reinterpret.
   * @return  The instance of a UPtr reinterpreted.
   */
  template<typename T, typename U>
  static FORCEINLINE UPtr<T> uniqueReinterpretCast(const UPtr<U>&& ptr);

  /**
   * @brief   Releases the memory of a shared pointer.
   * @param     ptr The object to be released.
   * @return  True if it succeeded to release.
   */
  template<typename T>
  static FORCEINLINE bool safeSharedRelease(SPtr<T>& ptr);
  /**
   * @brief   Releases the memory of a unique pointer.
   * @param     ptr The object to be released.
   * @return  True if it succeeded to release.
   */
  template<typename T>
  static FORCEINLINE bool safeUniqueRelease(UPtr<T>& ptr);
  /**
   * @brief   Releases the memory of a raw pointer.
   * @param     ptr The object to be released.
   * @return  True if it succeeded to release.
   */
  template<typename T>
  static FORCEINLINE bool rawSafeRelease(T** ptr);

 private:
};

template <typename T, typename... Args>
FORCEINLINE UPtr<T> MemoryManager::createUnique(Args&&... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}
template <typename T, typename... Args>
FORCEINLINE SPtr<T> MemoryManager::createShared(Args&&... args)
{
  return std::make_shared<T>(forward<Args>(args)...);
}

template<typename T, typename U>
FORCEINLINE SPtr<T> MemoryManager::sharedStaticCast(const SPtr<U>& ptr)
{
  return std::static_pointer_cast<T>(ptr);
}
template<typename T, typename U>
FORCEINLINE SPtr<T> MemoryManager::sharedDynamicCast(const SPtr<U>& ptr)
{
  return std::dynamic_pointer_cast<T>(ptr);
}
template<typename T, typename U>
FORCEINLINE SPtr<T> MemoryManager::sharedConstCast(const SPtr<U>& ptr)
{
  return std::const_pointer_cast<T>(ptr);
}
template<typename T, typename U>
FORCEINLINE SPtr<T> MemoryManager::sharedReinterpretCast(const SPtr<U>& ptr)
{
  return std::reinterpret_pointer_cast<T>(ptr);
}

template <typename T, typename U>
FORCEINLINE UPtr<T> MemoryManager::uniqueReinterpretCast(const UPtr<U>&& ptr)
{
  auto raw = reinterpret_cast<T*>(ptr.release());
  return std::unique_ptr<T>(raw);
}

template<typename T>
FORCEINLINE bool MemoryManager::safeSharedRelease(SPtr<T>& ptr)
{
  if (ptr) {
    ptr->release();
    return true;
  }
  return false;
}
template<typename T>
FORCEINLINE bool MemoryManager::safeUniqueRelease(UPtr<T>& ptr)
{
  if (ptr) {
    ptr->release();
    return true;
  }
  return false;
}
template<typename T> inline bool MemoryManager::rawSafeRelease(T** ptr)
{
  if (*ptr)
  {
    delete *ptr;
    *ptr = nullptr;
    return true;
  }
  return false;
}