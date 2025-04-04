#pragma once

#include "platform/defines.h"
#include "platform/stdHeaders.h"
#include "platform/math.h"

namespace eOPEN_TYPE {
  /**
   * @brief The flags for opening a file.
   */
  enum E : U8
  {
    kNone = 0,
    kReadOnly = 1u,
    kWriteOnly = 2u,
    kReadWrite = 4u,
    kAppend = 8u,
    kTruncate = 16u,
    kAtTheEnd = 32u,
    kBinary = 64u
  };
}

/**
 * @brief The file class, to wrap the file actions.
 */
class File
{
 public:
  /**
   * @brief Default constructor
   */
  File() = default;
  /**
   * @brief Opens a file path and stores it.
   * @param   path       The path of the file.
   * @param   openFlags  The flags to open the file.
   */
  File(const WString& path, U8 openFlags = 0u);
  /**
   * @brief Default destructor
   */
  ~File();

  /**
   * @brief Opens a file path and stores it.
   *
   * @param   path       The path of the file.
   * @param   openFlags  The flags to open the file.
   */
  void openFile(const WString& path, U8 openFlags = 0u);
  /**
   * @brief Close the file.
   */
  void close();
  /**
   * @brief  Return the current opened status of the file.
   * @return If the file is successfully opened.
   */
  bool isOpen() const;
  /**
   * @brief  Returns true if the file pointer is at the end of the file, so
   *         there are no more bytes or chars to be read.
   * @return If the file pointer is at the end of the file.
   */
  bool isAtEnd() const;


  /**
   * @brief Writes the given content on the file. The type of the data needs to have
   *        overwritten the << operator for fstreams.
   * @param   data  The content to write on the file
   */
  template<typename T>
  void write(const T data);
  /**
   * @brief Writes the given bytes array content on the file.
   * @param   data        The content to write on the file.
   * @param   bytesCount  The size of the data array.
   */
  void writeBytes(Byte* data, U32 bytesCount);
  /**
   * @brief Reads the file to the given variable. The type of the data needs to have
   *        overwritten the >> operator for fstreams.
   * @param   data  The variable to read the file on.
   */
  template<typename T>
  void read(T& data);
  /**
   * @brief Reads the file to the given variable as bytes.
   * @param   data  The array of bytes to store the data.
   * @param   bytesCount  The size of the data array.
   */
  void
  readBytes(Byte* data, U32 bytesCount);
  /**
   * @brief  Returns the hole content of the file.
   * @return The hole content of the file.
   */
  String
  getContent();
  /**
   * @brief  Returns the next line of the file.
   * @return The next line of the file.
   */
  String
  getLine(const char cutLine = '\n');
  /**
   * @brief  Returns the next char of the file.
   * @return The next char of the file.
   */
  char
  getNextChar();

  /**
   * @brief  Check the given path for any file or directory inside and return
   *         all of their paths in a list.
   * @param    path       The path to get the children paths from.
   * @param    recursive  If this is true, the function will enter any directory
   *                      in the path and return all the files inside them,
   *                      instead of returning the paths of the directories
   *                      itself.
   * @return The paths inside the given path.
   */
  static Vector<Path> getChildPaths(const WString& path, bool recursive = false);

 private:
  /**
   * @brief The file stored.
   */
  FStream m_file;
  /**
   * @brief The flags for the file.
   */
  U8 m_flags = 0;
};
template<typename T>
FORCEINLINE void
File::write(T data)
{
  if (!m_file.is_open() || m_file.fail()
    || Math::hasFlag(m_flags, eOPEN_TYPE::kReadOnly)) {
    return;
  }

  m_file << data;
}
template<typename T>
FORCEINLINE void
File::read(T& data)
{
  if (!m_file.is_open() || m_file.fail()
    || Math::hasFlag(m_flags, eOPEN_TYPE::kWriteOnly)) {
    return;
  }

  m_file >> data;
}