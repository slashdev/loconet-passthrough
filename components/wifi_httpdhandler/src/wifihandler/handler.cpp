#include "handler.hpp"

#include <cstring>
#include <unordered_map>
#include "tcpip_adapter.h"
#include "esp_err.h"
#include "esp_log.h"

#include "httpd/util/uri.hpp"

namespace httpd
{
  namespace handlers
  {
    namespace wifi
    {

      std::string Handler::GET_url()
      {
        return get_url_;
      }

      void Handler::GET_url(std::string url)
      {
        get_url_ = url;
      }

      std::string Handler::POST_url()
      {
        return post_url_;
      }

      void Handler::POST_url(std::string url)
      {
        post_url_ = url;
      }

      bool Handler::accept(httpd::Request* request)
      {
        // Accept: GET /wifi
        if (request->method() == httpd::method::GET && request->uri().compare(GET_url()) == 0)
        {
          return true;
        }
        else if (request->method() == httpd::method::POST && request->uri().compare(POST_url()) == 0)
        {
          return true;
        }
        return false;
      }

      void Handler::handle(httpd::Request* request, httpd::Response* response)
      {
        if (request->method() == httpd::method::GET && request->uri().compare(GET_url()) == 0)
        {
          std::string body = "<html><h1>WiFi Configuration</h1><form action=\"";
          body += POST_url();
          body += "\" method=\"post\"><table>";
          body += "<tr><td>SSID:</td><td><input type=\"text\" name=\"ssid\" maxlength=\"32\" value=\"";
          // body += WifiConnector::station()->ssid();
          body += "\"/></td></tr><tr><td>Password:</td><td><input type=\"password\" name=\"password\" maxlength=\"64\"/></td></tr>";
          body += "<tr><td></td><td><input type=\"submit\" name=\"submit\" value=\"submit\"/></td></tr></form></table>";

          body += "<h1>Current IP settings</h1>";
          tcpip_adapter_ip_info_t ip_info = {};
          esp_err_t err = tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
          if (err == ESP_OK)
          {
            body += "<table><tr><td>IP:</td><td>";
            body += ip4addr_ntoa(&ip_info.ip);
            body += "</td></tr><tr><td>Netmask:</td><td>";
            body += ip4addr_ntoa(&ip_info.netmask);
            body += "</td></tr><tr><td>Gateway:</td><td>";
            body += ip4addr_ntoa(&ip_info.gw);
            body += "</td></tr></table>";
          }

          response->status(httpd::status::OK);
          response->body(body);
        }
        else if (request->method() == httpd::method::POST && request->uri().compare(POST_url()) == 0)
        {
          ESP_LOGI("WifiRH", "Parsing string: %s", request->body().c_str());
          std::unordered_map<std::string, std::string> vars = httpd::uri::parse_query_string(request->body());

          if (    (vars.find("ssid") != vars.end())
               && (vars.find("password") != vars.end()) )
          {

            ESP_LOGI("WifiRH", "New settings: SSID: '%s', PW: '%s'",
              vars["ssid"].c_str(), vars["password"].c_str());

          }

          response->status(httpd::status::OK);
          std::string body = "<html><head><meta http-equiv=\"refresh\" content=\"10; url=/wifi\" /></head><body>The Wifi is being reset</html>";
          response->body(body);
        }
      }
    }
  }
}
