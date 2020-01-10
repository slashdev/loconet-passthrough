#include "event_handlers.hpp"
#include "wifi_connector.hpp"

WifiConnector::WifiConnector()
{
  ap_ = new WifiAP();
  station_ = new WifiStation();

  EventHandlers::add(WIFI_EVENT, this);
  EventHandlers::add(IP_EVENT, this);
}

WifiAP* WifiConnector::ap()
{
  return instance()->ap_;
}

WifiStation* WifiConnector::station()
{
  return instance()->station_;
}

bool WifiConnector::is_ap_ = false;

void WifiConnector::start()
{
  if (station()->ssid().empty())
  {
    is_ap_ = true;
    ap()->connect();
  }
  else
  {
    is_ap_ = false;
    station()->connect();
  }
}

void WifiConnector::stop()
{
  if (is_ap_)
  {
    ap()->disconnect();
  }
  else
  {
    station()->disconnect();
  }
}

void WifiConnector::handle_event(esp_event_base_t base, int32_t event, void *data)
{
  if (base == IP_EVENT && event == IP_EVENT_STA_GOT_IP)
  {
    // We are connected!
    tries_ = 0;
  }
  if (base == WIFI_EVENT && event == WIFI_EVENT_STA_DISCONNECTED)
  {
      tries_++;
      if (tries_ > WIFI_MAX_CONNECTION_RETRY)
      {
        is_ap_ = true;
        station_->disconnect();
        ap_->connect();
      }
  }
}


WifiConnector* WifiConnector::instance_ = NULL;

WifiConnector* WifiConnector::instance()
{
  if (instance_ == NULL)
  {
    instance_ = new WifiConnector();
  }
  return instance_;
}
