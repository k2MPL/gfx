#include "interface_FileSystem.h"

FileSystem::FileSystem() : m_Implementation{interface_FileSystem::Create()} {}

std::string FileSystem::readTextFromFile(const std::string &_path) {
  return m_Implementation->readTextFromFile(_path);
}

std::vector<unsigned char>
FileSystem::readBinaryFromFile(const std::string &_path) {
  return m_Implementation->readBinaryFromFile(_path);
}