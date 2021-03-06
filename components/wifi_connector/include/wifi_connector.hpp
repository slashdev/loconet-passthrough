#ifndef WIFI_CONNECTOR_HPP_
#define WIFI_CONNECTOR_HPP_

#include "event_handler.hpp"

#include "wifi_ap.hpp"
#include "wifi_station.hpp"

#ifndef WIFI_MAX_CONNECTION_RETRY
#define WIFI_MAX_CONNECTION_RETRY 4
#endif

class WifiConnector : public EventHandler
{
public:

  static WifiAP* ap();
  static WifiStation* station();

  static void start();
  static void stop();

  void handle_event(esp_event_base_t, int32_t, void*);

  static WifiConnector* instance();

private:
  WifiConnector();

  static bool is_ap_;
  static WifiConnector* instance_;

  WifiAP* ap_;
  WifiStation* station_;

  uint8_t tries_ = 0;
};


#endif // WIFI_CONNECTOR_HPP_
