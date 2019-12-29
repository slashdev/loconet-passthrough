#include "StringHandler.hpp"
#include "esp_log.h"
#include <cstring>



StringHandler::StringHandler(const char* uri)
{
  this->uri_ = uri;
  this->text_ = "";
}


StringHandler::StringHandler(const char* uri, const char* text)
{
  this->uri_ = uri;
  this->text_ = text;
}


const char * StringHandler::getUri()
{
  return this->uri_;
}

const char* StringHandler::getContent()
{
  return this->text_;
}


esp_err_t StringHandler::handle(httpd_req_t *req)
{
  /*
  char*  buf;
  size_t buf_len;
  
  // Get header value string length and allocate memory for length + 1,
  // extra byte for null termination
  buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
  if (buf_len > 1) 
  {
    buf = (char *) malloc(buf_len);
    // Copy null terminated value string into buffer
    if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK) 
    {
      ESP_LOGI(this->TAG, "Found header => Host: %s", buf);
    }
    free(buf);
  }

  // Read URL query string length and allocate memory for length + 1,
  // extra byte for null termination
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) 
  {
    buf = (char *) malloc(buf_len);
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) 
    {
      ESP_LOGI(this->TAG, "Found URL query => %s", buf);
      char param[32];
      // Get value of expected key from query string
      if (httpd_query_key_value(buf, "query1", param, sizeof(param)) == ESP_OK) 
      {
        ESP_LOGI(this->TAG, "Found URL query parameter => query1=%s", param);
      }
      if (httpd_query_key_value(buf, "query3", param, sizeof(param)) == ESP_OK) 
      {
        ESP_LOGI(this->TAG, "Found URL query parameter => query3=%s", param);
      }
      if (httpd_query_key_value(buf, "query2", param, sizeof(param)) == ESP_OK) 
      {
        ESP_LOGI(this->TAG, "Found URL query parameter => query2=%s", param);
      }
    }
    free(buf);
  }
  */
  const char* data = this->getContent();
  httpd_resp_send(req, data, strlen(data));

  return ESP_OK;
}