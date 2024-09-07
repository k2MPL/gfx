#pragma once

#include <memory>
#include <string>
#include <vector>

class interface_FileSystem {
public:
  virtual ~interface_FileSystem() = default;
  virtual std::string readTextFromFile(const std::string &_path) = 0;

  virtual std::vector<unsigned char>
  readBinaryFromFile(const std::string &_path) = 0;

  // Platform specific implementation
  static std::unique_ptr<interface_FileSystem> Create();
};

class FileSystem {
public:
  FileSystem();

  std::string readTextFromFile(const std::string &_path);
  std::vector<unsigned char> readBinaryFromFile(const std::string &_path);

private:
  std::unique_ptr<interface_FileSystem> m_Implementation;
};