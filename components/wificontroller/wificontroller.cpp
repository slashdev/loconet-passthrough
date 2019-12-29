#include "wificontroller.hpp"

namespace communication
{
  namespace wifi
  {

    Controller::Controller()
    {
      connected_ = false;
      ap_ = false;

      cfg_station_ = {};
      this->set_ssid(WIFICONNECTOR_SSID);
      this->set_password(WIFICONNECTOR_PASSWORD);
      this->set_max_retry_connects(WIFICONNECTOR_MAX_RETRIES);

      cfg_ap_ = {};
      cfg_ap_.ap.ssid_hidden = 0;
      this->set_ap_ssid(WIFICONNECTOR_AP_SSID);
      this->set_ap_password(WIFICONNECTOR_AP_PASSWORD);
      this->set_ap_channel(WIFICONNECTOR_AP_CHANNEL);
      this->set_ap_authmode(WIFI_AUTH_WPA_WPA2_PSK);
      this->set_ap_max_connections(WIFICONNECTOR_AP_MAX_CONNECTIONS);
    }

    bool Controller::is_connected()
    {
      return connected_;
    }
    bool Controller::is_ap()
    {
      return ap_;
    }

    Controller* Controller::set_ssid(std::string ssid) 
    {
      if (ssid.length() < 32) {
        std::copy(ssid.begin(), ssid.end(), cfg_station_.sta.ssid);
        station_ssid_ = ssid;
      } 
      else
      {
        station_ssid_ = "";
      }

      return this;
    }

    std::string Controller::get_ssid()
    {
      return station_ssid_;
    }

    Controller* Controller::set_password(std::string password) 
    {
      if (password.length() < 64) {
        std::copy(password.begin(), password.end(), cfg_station_.sta.password);
      }

      return this;
    }

    Controller* Controller::set_max_retry_connects(uint8_t tries)
    {
      max_retries_ = tries;
      return this; 
    }

    Controller* Controller::set_ap_ssid(std::string ssid) 
    {
      if (ssid.length() < 32) {
        std::copy(ssid.begin(), ssid.end(), cfg_ap_.ap.ssid);
      }

      return this;
    }

    Controller* Controller::set_ap_password(std::string password) 
    {
      if (password.length() < 64) {
        std::copy(password.begin(), password.end(), cfg_ap_.ap.password);
      }

      return this;
    }

    Controller* Controller::set_ap_authmode(wifi_auth_mode_t auth)
    {
      cfg_ap_.ap.authmode = auth;
      return this;
    }

    Controller* Controller::set_ap_channel(uint8_t channel)
    {
      cfg_ap_.ap.channel = channel;
      return this;
    }

    Controller* Controller::set_ap_max_connections(uint8_t connections)
    {
      cfg_ap_.ap.max_connection = connections;
      return this;
    }

    void Controller::initialize()
    {
      wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
      ESP_ERROR_CHECK(esp_wifi_init(&cfg)); 
    }

    void Controller::connect()
    {
      // If the SSID is set, try to connect to the AP
      // If the SSID is not set, directly become an AP
      if (this->get_ssid().empty()) 
      {
        this->become_ap();
      }
      else
      {
        this->become_station();
      }
    }

    void Controller::disconnect()
    {
      ESP_ERROR_CHECK(esp_wifi_stop() );
    }

    void Controller::reset()
    {
      this->disconnect();
      this->connect();
    }

    tcpip_adapter_ip_info_t Controller::get_ip()
    {
      return ip_info_;
    }

    esp_err_t Controller::event_handler(void *ctx, system_event_t *event)
    {
      switch(event->event_id) {
          // Event thrown if ready to connect
        case SYSTEM_EVENT_STA_START:
          esp_wifi_connect();
          break;
          // We got an IP address, hence we are connected!
        case SYSTEM_EVENT_STA_GOT_IP:
          // Reset the counter for retries
          retry_num_ = 0;
          // TODO: Set IP-variable
          // &event->event_info.got_ip.ip_info.ip));
          tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info_);
          // We are now connected! 
          connected_ = true;

          this->notify_connect();
          break;
          // If we got disconnected, go and retry
        case SYSTEM_EVENT_STA_DISCONNECTED:
          {
            connected_ = false;
            ip_info_ = {};
            // TODO Only notify the first time that the disconnect happens
            this->notify_disconnect();

            if (retry_num_ < max_retries_) {
                esp_wifi_connect();
                retry_num_++;
            }
            else
            {
                // We need to become an AP ourselves
                // 1. stop the wifi
                this->disconnect();
                // 2. become an access point
                this->become_ap();
            }
          }   
          break;
        // We become an AP
        case SYSTEM_EVENT_AP_START:
          ap_ = true;
          this->notify_start_as_ap();
          break;
        // In case a station connects to this AP
        case SYSTEM_EVENT_AP_STACONNECTED:
          break;
        // In case a station disconnects to this AP
        case SYSTEM_EVENT_AP_STADISCONNECTED:
          break;
        default:
          break;
      }

      return ESP_OK;
    }

    void Controller::become_station()
    {
      // We set the mode to Station 
      ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
      // Load the config
      ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &cfg_station_) );
      // Start the wifi
      ESP_ERROR_CHECK(esp_wifi_start() );
    }

    void Controller::become_ap()
    {
      ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
      ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &cfg_ap_) );
      // Start the wifi
      ESP_ERROR_CHECK(esp_wifi_start() );
    }


    void Controller::register_listener(ConnectionListener* listener)
    {
      listeners_.push_back(listener);
    }

    void Controller::unregister_listener(ConnectionListener* listener)
    {
      listeners_.remove(listener);
    }

    void Controller::notify_connect()
    {
      for(auto const&l : listeners_)
      {
        l->on_connect();
      }
    }

    void Controller::notify_disconnect()
    {
      for(auto const&l : listeners_)
      {
        l->on_disconnect();
      }
    }

    void Controller::notify_start_as_ap()
    {
      for(auto const&l : listeners_)
      {
        l->on_start_as_ap();
      }
    }

  } // End of namespace wifi
} // End of namespace communication