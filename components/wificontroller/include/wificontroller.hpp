#ifndef _BOOTSTRAP_WIFI_H_
#define _BOOTSTRAP_WIFI_H_

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"

#include "tcpip_adapter.h"

#include <string>
#include <list>
#include "wificontroller_listener.hpp"

#ifndef WIFICONNECTOR_MAX_RETRIES
#define WIFICONNECTOR_MAX_RETRIES 4
#endif

#ifndef WIFICONNECTOR_SSID
#define WIFICONNECTOR_SSID "defaultSSID"
#endif

#ifndef WIFICONNECTOR_PASSWORD
#define WIFICONNECTOR_PASSWORD "defaultPassword"
#endif

#ifndef WIFICONNECTOR_AP_SSID
#define WIFICONNECTOR_AP_SSID "sd_loconet"
#endif

#ifndef WIFICONNECTOR_AP_PASSWORD
#define WIFICONNECTOR_AP_PASSWORD "Aa123456"
#endif

#ifndef WIFICONNECTOR_AP_CHANNEL
#define WIFICONNECTOR_AP_CHANNEL 0
#endif

#ifndef WIFICONNECTOR_AP_MAX_CONNECTIONS
#define WIFICONNECTOR_AP_MAX_CONNECTIONS 1
#endif

namespace communication
{
  namespace wifi
  {

    class Controller
    {
    public:
      Controller();
      ~Controller();

      bool is_connected();
      bool is_ap();

      Controller* set_ssid(std::string);
      std::string get_ssid();

      Controller* set_password(std::string);

      Controller* set_max_retry_connects(uint8_t);

      Controller* set_ap_ssid(std::string);
      Controller* set_ap_password(std::string);
      Controller* set_ap_authmode(wifi_auth_mode_t);
      Controller* set_ap_channel(uint8_t);
      Controller* set_ap_max_connections(uint8_t);

      void initialize();

      void connect();
      void disconnect();

      void reset();

      tcpip_adapter_ip_info_t get_ip();

      esp_err_t event_handler(void *ctx, system_event_t *event);

      void register_listener(ConnectionListener*);
      void unregister_listener(ConnectionListener*);

    private:
      bool connected_ = false;
      bool ap_ = false;

      uint8_t retry_num_ = 0;
      uint8_t max_retries_ = 4;

      tcpip_adapter_ip_info_t ip_info_ = {};

      std::string station_ssid_ = {};
      wifi_config_t cfg_station_ = {};
      wifi_config_t cfg_ap_ = {};

      void become_station();
      void become_ap();

      std::list<ConnectionListener*> listeners_;

      void notify_connect();
      void notify_disconnect();
      void notify_start_as_ap();
    };

  } // End of namespace wifi
} // End of namespace communication

#endif
