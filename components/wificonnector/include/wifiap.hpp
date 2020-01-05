#ifndef WIFIAP_HPP_
#define WIFIAP_HPP_

#include "wifibase.hpp"

class WifiAP : public WifiBase
{
public:
  WifiAP();

  void ssid(std::string);
  void password(std::string);

  void authmode(wifi_auth_mode_t);
  wifi_auth_mode_t authmode();

  void channel(uint8_t);
  uint8_t channel();

  void max_connections(uint8_t);
  uint8_t max_connections();

  void connect();

  void handle_event(esp_event_base_t, int32_t, void *);
private:
  wifi_config_t cfg_ap_ = {};

};


#endif
