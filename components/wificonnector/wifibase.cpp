#include "wifibase.hpp"

WifiBase::WifiBase()
{
  cfg_ = WIFI_INIT_CONFIG_DEFAULT();
}

WifiBase::~WifiBase()
{
  this->disconnect();
}

bool WifiBase::is_connected()
{
  return connected_;
}

std::string WifiBase::get_ssid()
{
  return ssid_;
}

void WifiBase::disconnect()
{
  connected_ = false;
  
  ESP_ERROR_CHECK(esp_wifi_stop() );
  // If this throws an error, we don't mind, as long as we 
  // prepare that the next can init again...
  esp_wifi_deinit();
}
