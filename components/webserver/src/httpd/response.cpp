#include "response.hpp"

#include "esp_log.h"

namespace httpd
{
  Response::Response()
  {
    // Set default to 404 - not found
    status_ = status::NOT_FOUND;
    header("Content-Type", "plain/text");
    body_ = std::string("");
  }

  void Response::status(status::eStatus_t code)
  {
    status_ = code;
  }

  status::eStatus_t Response::status()
  {
    return status_;
  }

  void Response::header(std::string key, std::string value)
  {
    headers_[key] = value;
  }

  std::string Response::header(std::string key)
  {
    auto element = headers_.find(key);
    if (element != headers_.end())
    {
      return element->second;
    }
    else
    {
      return "";
    }
  }

  void Response::body(const char* str)
  {
    body_ =std::string(str);
  }

  void Response::body(const char* str, size_t length)
  {
    body_ = std::string(str, length);
  }

  void Response::body(std::string str)
  {
    body_ = str;
  }

  std::string Response::body()
  {
    return body_;
  }

  std::string Response::message()
  {
    std::string msg = std::string("HTTP/1.1 ");
    msg += std::to_string(status_);
    msg += " ";
    msg += response_phrase(status_);
    msg += "\r\n";

    /*
    for(auto &h : headers_)
    {
      stream
          << h.first
          << ": "
          << h.second
          << "\r\n";
    }
    */
    msg += "Content-Length: " + std::to_string(body_.length());
    msg += "\r\n\r\n";

    ESP_LOGI("Request", "body: %s", body_.c_str());

    if (!body_.empty()) msg += body_;

    return msg;
    // return msg.c_str();
  }

  void Response::construct_default_message(std::string msg)
  {
    std::string error = std::to_string(status());
    error += " - ";
    error += Response::response_phrase(status());

    body_ = std::string("<html><head><title>");
    body_ += error;
    body_ += "</title></head><body><h1>";
    body_ += error;
    body_ += "</h1>";
    body_ += msg;
    body_ += "</body></html>";

    header("Content-Type", "text/html");
  }

  const char* Response::response_phrase(status::eStatus_t code)
  {
    switch(code)
    {
      case status::CONTINUE                   : // CODE: 100
        return "Continue";
      case status::SWITCHING_PROTOCOLS        : // CODE: 101
        return "Switching Protocols";
      case status::OK                         : // CODE: 200
        return "OK";
      case status::CREATED                    : // CODE: 201
        return "Created";
      case status::ACCEPTED                   : // CODE: 202
        return "Accepted";
      case status::NON_AUTHORATIVE            : // CODE: 203
        return "Non Authorative Information";
      case status::NO_CONTENT                 : // CODE: 204
        return "No Content";
      case status::RESET_CONTENT              : // CODE: 205
        return "Reset Content";
      case status::PARTIAL_CONTENT            : // CODE: 206
        return "Partial Content";
      case status::MULTIPLE_CHOICE            : // CODE: 300
        return "Multiple Choice";
      case status::MOVED_PERMANENTLY          : // CODE: 301
        return "Moved Permanently";
      case status::FOUND                      : // CODE: 302
        return "Found";
      case status::SEE_OTHER                  : // CODE: 303
        return "See Other";
      case status::NOT_MODIFIED               : // CODE: 304
        return "Not Modified";
      case status::USE_PROXY                  : // CODE: 305
        return "Use Proxy";
      case status::TEMPORARY_REDIRECT         : // CODE: 307
        return "Temporary Redirect";
      case status::BAD_REQUEST                : // CODE: 400
        return "Bad Request";
      case status::UNAUTHORIZED               : // CODE: 401
        return "Unauthorized";
      case status::PAYMENT_REQUIRED           : // CODE: 402
        return "Payment Required";
      case status::FORBIDDEN                  : // CODE: 403
        return "Forbidden";
      case status::NOT_FOUND                  : // CODE: 404
        return "Not Found";
      case status::METHOD_NOT_ALLOWED         : // CODE: 405
        return "Method Not Allowed";
      case status::NOT_ACCEPTABLE             : // CODE: 406
        return "Not Acceptable`";
      case status::PROXY_AUTH_REQUIRED        : // CODE: 407
        return "Proxy Authentication Required";
      case status::REQUEST_TIME_OUT           : // CODE: 408
        return "Request Time Out";
      case status::CONFLICT                   : // CODE: 409
        return "Conflict";
      case status::GONE                       : // CODE: 410
        return "Gone";
      case status::LENGTH_REQUIRED            : // CODE: 411
        return "Length Required";
      case status::PRECONDITION_FAILED        : // CODE: 412
        return "Precondition Failed";
      case status::REQUEST_ENTITY_TOO_LARGE   : // CODE: 413
        return "Request Entity Too Large";
      case status::REQUEST_URI_TOO_LARGE      : // CODE: 414
        return "Request URI Too Large";
      case status::UNSUPPORTED_MEDIA_TYPE     : // CODE: 415
        return "Unsupported Media Type";
      case status::REQUESTED_RANGE_NOT_SAT    : // CODE: 416
        return "Requested Range Not Satisfiable";
      case status::EXPECTATION_FAILED         : // CODE: 417
        return "Expectation Failed";
      case status::INTERNAL_SERVER_ERROR      : // CODE: 500
        return "Internal Server Error";
      case status::NOT_IMPLEMENTED            : // CODE: 501
        return "Not Implemented";
      case status::BAD_GATEWAY                : // CODE: 502
        return "Bad Gateway";
      case status::SERVICE_UNAVAILABLE        : // CODE: 503
        return "Service Unavailable";
      case status::GATEWAY_TIME_OUT           : // CODE: 504
        return "Gateway Time Out";
      case status::HTTP_VERSION_NOT_SUPPORTED : // CODE: 505
        return "HTTP Version Not Supported";
      default:
        return "";
    }
  }
}
