#include "desktop_FileSystem.h"

#include <fstream>
#include <streambuf>

std::unique_ptr<interface_FileSystem> interface_FileSystem::Create() {
  return std::unique_ptr<interface_FileSystem>{new desktop_FileSystem{}};
}

std::string desktop_FileSystem::readTextFromFile(const std::string &_path) {
  std::ifstream file{"data/" + _path};
  if (file.is_open()) {
    return std::string{std::istreambuf_iterator<char>(file),
                       std::istreambuf_iterator<char>()};
  }

  return {};
}

std::vector<unsigned char>
desktop_FileSystem::readBinaryFromFile(const std::string &_path) {
  std::ifstream file{"data/" + _path, std::ios::binary};
  if (file.is_open()) {
    return std::vector<unsigned char>{std::istreambuf_iterator<char>(file),
                                      std::istreambuf_iterator<char>()};
  }

  return {};
}