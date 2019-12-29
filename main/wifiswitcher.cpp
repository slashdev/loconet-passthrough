#include "wifiswitcher.hpp"
#include "genericeventloop.hpp"

WifiSwitcher::WifiSwitcher()
{
  ap_ = new WifiAP();
  station_ = new WifiStation();

  EventLoop::get_instance()->register_handler(this);
  EventLoop::get_instance()->register_handler(ap_);
  EventLoop::get_instance()->register_handler(station_);
}

WifiAP* WifiSwitcher::ap()
{
  return instance()->ap_;
}

WifiStation* WifiSwitcher::station()
{
  return instance()->station_;
}

void WifiSwitcher::start()
{
  if (station()->get_ssid().empty())
  {
    ap()->connect();
  }
  else
  {
    station()->connect();
  }
}

esp_err_t WifiSwitcher::event_handler(void *ctx, system_event_t *event)
{
  switch(event->event_id)
  {
    case SYSTEM_EVENT_STA_GOT_IP:
      tries_ = 0;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      tries_++;
      if (tries_ > WIFI_MAX_CONNECTION_RETRY)
      {
        station_->disconnect();
        ap_->connect();
      }
      break;
    default:
      break;
  }
  return ESP_OK;
}



WifiSwitcher* WifiSwitcher::instance_ = NULL;

WifiSwitcher* WifiSwitcher::instance()
{
  if (instance_ == NULL)
  {
    instance_ = new WifiSwitcher();
  }
  return instance_;
}