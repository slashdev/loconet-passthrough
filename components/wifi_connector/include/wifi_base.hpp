#ifndef WIFI_BASE_HPP_
#define WIFI_BASE_HPP_

#include <string>

#include "esp_system.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"

#include "event_handler.hpp"

class WifiBase : public EventHandler
{
public:
  WifiBase();
  ~WifiBase();

  bool connected();

  std::string ssid();
  virtual void ssid(std::string) =0;

  virtual void password(std::string) =0;

  virtual void connect() =0;
  void disconnect();

protected:
  bool connected_ = false;
  wifi_init_config_t cfg_;
  std::string ssid_;
};

#endif // WIFI_BASE_HPP_
