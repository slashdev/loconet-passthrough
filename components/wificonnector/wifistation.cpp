#include "wifistation.hpp"

#include "event_handlers.hpp"

WifiStation::WifiStation()
{
  cfg_station_ = {};
  cfg_station_.sta = {};

  EventHandlers::add(WIFI_EVENT, this);
  EventHandlers::add(IP_EVENT, this);
}

void WifiStation::set_ssid(std::string ssid)
{
  if (ssid.length() < 32) {
    std::copy(ssid.begin(), ssid.end(), cfg_station_.sta.ssid);
    ssid_ = ssid;
  }
  else
  {
    ssid_ = "";
  }
}

void WifiStation::set_password(std::string password)
{
  if (password.length() < 64) {
    std::copy(password.begin(), password.end(), cfg_station_.sta.password);
  }
}

void WifiStation::connect()
{
  ESP_ERROR_CHECK(esp_wifi_init(&cfg_));

  // We set the mode to Station
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
  // Load the config
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &cfg_station_) );
  // Start the wifi
  ESP_ERROR_CHECK(esp_wifi_start() );
}

bool WifiStation::allow_retry()
{
  return allow_retry_;
}

void WifiStation::set_allow_retry(bool retry)
{
  allow_retry_ = retry;
}


void WifiStation::handle_event(esp_event_base_t base, int32_t event, void *data)
{
  if (base == WIFI_EVENT)
  {
    switch(event)
    {
      case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        break;
      case WIFI_EVENT_STA_DISCONNECTED:
        connected_ = false;
        if (allow_retry())
        {
            esp_wifi_connect();
        }
        break;
      default:
        break;
    }
  }
  if (base == IP_EVENT && event == IP_EVENT_STA_GOT_IP)
  {
    connected_ = true;
  }
}