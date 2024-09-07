#pragma once

#include <memory>
#include <string>


class interface_Logger {
public:
  virtual ~interface_Logger() = default;
  virtual void log(const std::string &_message) const = 0;

  // Platform specific implementation
  static std::unique_ptr<interface_Logger> Create();
};

class Logger final {
public:
  Logger();

  void log(const std::string &_message) const;

private:
  std::unique_ptr<interface_Logger> m_Implementation;
};