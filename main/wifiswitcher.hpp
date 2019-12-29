#ifndef WIFISWITCHER_HPP_
#define WIFISWITCHER_HPP_

#include "genericeventhandler.hpp"

#include "wifiap.hpp"
#include "wifistation.hpp"

#define WIFI_MAX_CONNECTION_RETRY 4

class WifiSwitcher : public EventHandler
{
public:
  
  static WifiAP* ap();
  static WifiStation* station();

  static void start();

  esp_err_t event_handler(void *, system_event_t *);

  static WifiSwitcher* instance();

private:
  WifiSwitcher();

  static WifiSwitcher* instance_;

  WifiAP* ap_;
  WifiStation* station_;

  uint8_t tries_ = 0;
};


#endif