#include "Webserver.hpp"
#include "esp_log.h"
#include "esp_panic.h"

Webserver::Webserver()
{
}


esp_err_t Webserver::register_handler(UriHandler* handler)
{
  httpd_uri_t uri = {};
  uri.uri = handler->getUri();
  uri.method = handler->getHttpMethod();
  uri.handler = Webserver::handle;
  uri.user_ctx = NULL;

  if (httpd_register_uri_handler(server_, &uri) == ESP_OK)
  {
    handlers_.push_back(handler);
  }

  return ESP_OK;
}

esp_err_t Webserver::start()
{
  server_ = NULL;
  config_ = HTTPD_DEFAULT_CONFIG();

  ESP_LOGW("Webserver", "Start server");

  httpd_start(&server_, &config_);

  return ESP_OK;
}

esp_err_t Webserver::stop()
{
  if (server_ != NULL) return httpd_stop(server_);
  return ESP_OK;
}

esp_err_t Webserver::handle(httpd_req_t *r)
{
  ESP_LOGI("SERVER", "I got a request for '%s'!", r->uri);
  for(auto const &h : Webserver::get_instance()->handlers_)
  {
    ESP_LOGI("SERVER", "Matching with handler: '%s'", h->getUri());
    int comparison = strcmp(h->getUri(), r->uri);
    if (comparison == 0)
    {
      h->handle(r);
      break;
    } 
    else
    {
      ESP_LOGW("SERVER", "No match (%d)", comparison);
    }
  }
  return ESP_OK;
}

Webserver* Webserver::instance_ = NULL;

Webserver* Webserver::get_instance()
{
  if (instance_ == NULL)
  {
    instance_ = new Webserver();
  }
  return instance_;
}