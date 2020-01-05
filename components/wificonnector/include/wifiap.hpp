#ifndef WIFIAP_HPP_
#define WIFIAP_HPP_

#include "wifibase.hpp"

class WifiAP : public WifiBase
{
public:
  WifiAP();

  void set_ssid(std::string);
  void set_password(std::string);

  void set_authmode(wifi_auth_mode_t);
  void set_channel(uint8_t);
  void set_max_connections(uint8_t);

  void connect();

  esp_err_t handle_event(void *, system_event_t *);
private:
  wifi_config_t cfg_ap_ = {};

};


#endif