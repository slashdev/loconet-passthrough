#ifndef WIFISTATION_HPP_
#define WIFISTATION_HPP_

#include "wifibase.hpp"

class WifiStation : public WifiBase
{
public:
  WifiStation();

  std::string ssid();
  void ssid(std::string);
  void password(std::string);

  bool allow_retry();
  void allow_retry(bool);

  void connect();

  void handle_event(esp_event_base_t, int32_t, void *);

private:
  wifi_config_t cfg_station_ = {};
  bool allow_retry_;
};


#endif
