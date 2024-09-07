#pragma once

#include <interface_Logger.h>

class desktop_Logger : public interface_Logger {
public:
  void log(const std::string &_message) const override;
};