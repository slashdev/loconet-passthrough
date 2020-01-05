#ifndef WIFISTATION_HPP_
#define WIFISTATION_HPP_

#include "wifibase.hpp"

class WifiStation : public WifiBase
{
public:
  WifiStation();

  void set_ssid(std::string);
  void set_password(std::string);

  bool allow_retry();
  void set_allow_retry(bool);

  void connect();

  esp_err_t handle_event(void *, system_event_t *);
private:
  wifi_config_t cfg_station_ = {};
  bool allow_retry_;
};


#endif