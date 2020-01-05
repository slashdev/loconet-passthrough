#include "wifiap.hpp"

WifiAP::WifiAP()
{
  cfg_ap_ = {};
  cfg_ap_.ap = {};
  cfg_ap_.ap.ssid_hidden = 0;

  // Default set WPA_WPA2 authmode
  this->set_authmode(WIFI_AUTH_WPA_WPA2_PSK);
  this->set_max_connections(4);
  this->set_channel(0);
}
  
void WifiAP::set_ssid(std::string ssid)
{
  if (ssid.length() < 32) {
    std::copy(ssid.begin(), ssid.end(), cfg_ap_.ap.ssid);
    ssid_ = ssid;
  } 
  else
  {
    ssid_ = "";
  }
}

void WifiAP::set_password(std::string password)
{
  if (password.length() < 64) {
    std::copy(password.begin(), password.end(), cfg_ap_.ap.password);
  }
}

void WifiAP::set_authmode(wifi_auth_mode_t mode)
{
  cfg_ap_.ap.authmode = mode;
}

void WifiAP::set_channel(uint8_t channel)
{
  cfg_ap_.ap.channel = channel;
}

void WifiAP::set_max_connections(uint8_t connections)
{
  cfg_ap_.ap.max_connection = connections;
}

void WifiAP::connect()
{
  ESP_ERROR_CHECK(esp_wifi_init(&cfg_)); 

  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &cfg_ap_) );
  // Start the wifi
  ESP_ERROR_CHECK(esp_wifi_start() );
}

esp_err_t WifiAP::handle_event(void *ctx, system_event_t *event)
{
  switch(event->event_id)
  {
    // We become an AP
    case SYSTEM_EVENT_AP_START:
      connected_ = true;
      break;
    default:
      break;
  }
  return ESP_OK;
}
