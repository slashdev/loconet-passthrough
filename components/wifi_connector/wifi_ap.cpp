#include <algorithm>
#include "wifi_ap.hpp"
#include "event_handlers.hpp"

WifiAP::WifiAP()
{
  cfg_ap_ = {};
  cfg_ap_.ap = {};
  cfg_ap_.ap.ssid_hidden = 0;
  cfg_ap_.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

  this->max_connections(4);

  EventHandlers::add(WIFI_EVENT, this);
}

void WifiAP::ssid(std::string ssid)
{
  if (ssid.length() < 33)
  {
    std::copy(ssid.begin(), ssid.end(), cfg_ap_.ap.ssid);
    ssid_ = ssid;
  }
  else
  {
    std::copy_n(ssid.begin(), 32, cfg_ap_.ap.ssid);
  }
}

void WifiAP::password(std::string password)
{
  if (password.length() < 65)
  {
    std::copy(password.begin(), password.end(), cfg_ap_.ap.password);
  }
  else
  {
    std::copy_n(password.begin(), 64, cfg_ap_.ap.password);
  }
}

void WifiAP::max_connections(uint8_t connections)
{
  cfg_ap_.ap.max_connection = connections;
}

uint8_t WifiAP::max_connections()
{
  return cfg_ap_.ap.max_connection;
}

void WifiAP::connect()
{
  ESP_ERROR_CHECK(esp_wifi_init(&cfg_));

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &cfg_ap_) );
  // Start the wifi
  ESP_ERROR_CHECK(esp_wifi_start() );
}

void WifiAP::handle_event(esp_event_base_t base, int32_t event, void *data)
{
  if (base == WIFI_EVENT)
  {
    switch(event)
    {
      // We become an AP
      case WIFI_EVENT_AP_START:
        connected_ = true;
        break;
      case WIFI_EVENT_AP_STOP:
        connected_ = false;
        break;
      default:
        break;
    }
  }
}
