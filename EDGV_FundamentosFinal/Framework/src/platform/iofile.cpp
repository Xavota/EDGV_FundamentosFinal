#include "platform/iofile.h"

File::File(const WString& path, U8 openFlags)
{
  openFile(path, openFlags);
}
File::~File()
{
  close();
}
void File::openFile(const WString& path, U8 openFlags)
{
  if (m_file.is_open()) {
    m_file.close();
  }

  bool hasOneReadWrite = false;
  if (Math::hasFlag(openFlags, eOPEN_TYPE::kReadOnly)) {
    hasOneReadWrite = true;
  }
  if (Math::hasFlag(openFlags, eOPEN_TYPE::kWriteOnly)) {
    if (hasOneReadWrite) return;
    hasOneReadWrite = true;
  }
  if (Math::hasFlag(openFlags, eOPEN_TYPE::kReadWrite)) {
    if (hasOneReadWrite) return;
  }

  m_flags = openFlags;
  if (!hasOneReadWrite) {
    m_flags |= eOPEN_TYPE::kReadWrite;
  }

  U8 mode = 0;
  if (Math::hasFlag(m_flags, eOPEN_TYPE::kAppend)) {
    mode |= std::ios_base::app;
  }
  if (Math::hasFlag(m_flags, eOPEN_TYPE::kTruncate)) {
    mode |= std::ios_base::trunc;
  }
  if (Math::hasFlag(m_flags, eOPEN_TYPE::kAtTheEnd)) {
    mode |= std::ios_base::ate;
  }
  if (Math::hasFlag(m_flags, eOPEN_TYPE::kBinary)) {
    mode |= std::ios_base::binary;
  }
  if (Math::hasFlag(m_flags, eOPEN_TYPE::kReadOnly)) {
    mode |= std::ios_base::in;
  }
  if (Math::hasFlag(m_flags, eOPEN_TYPE::kWriteOnly)) {
    mode |= std::ios_base::out;
  }
  if (Math::hasFlag(m_flags, eOPEN_TYPE::kReadWrite)) {
    mode |= std::ios_base::out | std::ios_base::in;
  }
  

  m_file.open(path, mode);

  if (!m_file || m_file.fail()) {
    m_file.close();
  }
}

void File::close()
{
  if (m_file.is_open()) {
    m_file.close();
  }
}

bool File::isOpen() const
{
  return m_file.is_open();
}

void File::writeBytes(Byte* data, U32 bytesCount)
{
  if (!m_file.is_open() || m_file.fail()
    || Math::hasFlag(m_flags, eOPEN_TYPE::kReadOnly)) {
    return;
  }

  m_file.write(reinterpret_cast<char*>(data), bytesCount);
}

void File::readBytes(Byte* data, U32 bytesCount)
{
  if (!m_file.is_open() || m_file.fail()
    || Math::hasFlag(m_flags, eOPEN_TYPE::kWriteOnly)) {
    return;
  }

  m_file.read(reinterpret_cast<char*>(data), bytesCount);
}

String File::getContent()
{
  if (!m_file.is_open() || m_file.fail()
  || Math::hasFlag(m_flags, eOPEN_TYPE::kWriteOnly)) {
    return "";
  }

  String content;
  while (!m_file.eof()) {
    content += getLine();
    content += "\n";
  }
  return content;
}

String File::getLine(char cutLine)
{
  if (!m_file.is_open() || m_file.fail()
  || Math::hasFlag(m_flags, eOPEN_TYPE::kWriteOnly)) {
    return "";
  }

  String line;
  std::getline(m_file, line, cutLine);
  return line;
}
char File::getNextChar()
{
  if (!m_file.is_open() || m_file.fail()
  || Math::hasFlag(m_flags, eOPEN_TYPE::kWriteOnly)) {
    return 0;
  }

  char c;
  m_file >> c;
  return c;
}