#ifndef WIFIBASE_HPP_
#define WIFIBASE_HPP_

#include "genericeventhandler.hpp"
#include <string>
#include "esp_system.h"
#include "esp_wifi.h"
#include "tcpip_adapter.h"


class WifiBase : public EventHandler
{
public:
  WifiBase();
  ~WifiBase();
  
  bool is_connected();

  virtual void set_ssid(std::string) =0;
  virtual void set_password(std::string) =0;

  std::string get_ssid();

  virtual void connect() =0;
  void disconnect();

protected:
  bool connected_ = false;
  wifi_init_config_t cfg_;
  std::string ssid_;
};

#endif