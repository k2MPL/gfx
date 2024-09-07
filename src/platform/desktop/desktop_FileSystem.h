#pragma once

#include <interface_FileSystem.h>

class desktop_FileSystem : public interface_FileSystem {
private:
  std::string readTextFromFile(const std::string &_path) override;

  std::vector<unsigned char>
  readBinaryFromFile(const std::string &_path) override;
};