#include <interface_Logger.h>

Logger::Logger() : m_Implementation{interface_Logger::Create()} {}

void Logger::log(const std::string &_message) const {
  m_Implementation->log(_message);
}