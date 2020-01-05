#ifndef WIFIAP_HPP_
#define WIFIAP_HPP_

#include "wifi_base.hpp"

class WifiAP : public WifiBase
{
public:
  WifiAP();

  void ssid(std::string);
  void password(std::string);

  void max_connections(uint8_t);
  uint8_t max_connections();

  void connect();

  void handle_event(esp_event_base_t, int32_t, void *);
private:
  wifi_config_t cfg_ap_ = {};

};


#endif
