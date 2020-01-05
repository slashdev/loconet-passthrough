#include "wifirequesthandler.hpp"

#include <cstring>
#include <string>

#include "tcpip_adapter.h"

bool WifiRequestHandler::accept(httpd::Request* request)
{
  if (request->method() == httpd::method::GET && request->uri().compare("/wifi") == 0)
  {
    return true;
  }
  else if (request->method() == httpd::method::POST && request->uri().compare("/wifi/set") == 0)
  {
    return true;
  }
  return false;
}

void WifiRequestHandler::handle(httpd::Request* request, httpd::Response* response)
{
  if (request->method() == httpd::method::GET && request->uri().compare("/wifi") == 0)
  {
    std::string body = "<html><h1>WiFi Configuration</h1><form action=\"/wifi/set\" method=\"post\"><table>";
    body += "<tr><td>SSID:</td><td><input type=\"text\" name=\"ssid\" maxlength=\"32\" value=\"";
    // body += SSID VALUE
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
  else if (request->method() == httpd::method::POST && request->uri().compare("/wifi/set") == 0)
  {
    // TODO!
  }
}