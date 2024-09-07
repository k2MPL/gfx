#include <desktop_Logger.h>

#include <iostream>

std::unique_ptr<interface_Logger> interface_Logger::Create() {
  return std::unique_ptr<interface_Logger>{new desktop_Logger{}};
}

void desktop_Logger::log(const std::string &_message) const {
  std::cout << "[LOG]: " << _message << std::endl;
}