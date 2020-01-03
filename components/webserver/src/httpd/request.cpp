#include "request.hpp"

#include <cstring>

#include "esp_log.h"

namespace httpd
{

	Request::~Request()
	{
		// ESP_LOGW("Request", "I am being destroyed");
	}

	// NOTE This function expects a 0-terminated String!
	Request::Request(char *data)
	{

    method_ = method::OTHER;
    uri_ = "";

		char *cursor = data;

		// Parse the method
		cursor = parse_method(cursor);
		// ESP_LOGI("Request", "I have method: '%s'", std::string(data, cursor).c_str());

		// Parse the URI
		cursor = parse_uri(cursor);
		// ESP_LOGI("Request", "I have uri: %s", uri_.c_str());

		// Move to the end of the line
		cursor = std::strstr(cursor, "\r\n");
		cursor += 2;

		// Parse all headers
		cursor = parse_headers(cursor );
		// If the cursor is not at the end, there is still
		// at least one \n, so we remove that one as well.

		if (*cursor != 0)
		{
			body_ = std::string(cursor);
		}
		else
		{
			body_ = std::string("");
		}
	}

	method::eHTTPMethod_t Request::method()
	{
		return method_;
	}

	std::string Request::uri()
	{
		return uri_;
	}

	std::string Request::header(std::string key)
	{
		auto item = headers_.find(key);
		if(item == headers_.end())
		{
			return "";
		}
		else
		{
			return item->second;
		}
	}

	std::string Request::body()
	{
		return body_;
	}

	char* Request::parse_method(char* cursor)
	{
		// Move to the next space...
		char* end = cursor;
		int count = 0;
		while(*end && *end != ' ')
		{
			end++;
			count++;
		}

		method_ = method::OTHER;

		if (count == 3)
		{
			if      (0 == std::strncmp(cursor, "GET", 3)) method_ = method::GET;
			else if (0 == std::strncmp(cursor, "PUT", 3)) method_ = method::PUT;
		}
		else if (count == 4)
		{
			if      (0 == std::strncmp(cursor, "POST", 4)) method_ = method::POST;
			else if (0 == std::strncmp(cursor, "HEAD", 4)) method_ = method::HEAD;
		}
		else if (count == 5)
		{
			if      (0 == std::strncmp(cursor, "TRACE", 5)) method_ = method::TRACE;
		}
		else if (count == 6)
		{
			if      (0 == std::strncmp(cursor, "DELETE", 6)) method_ = method::DELETE;
		}
		else if (count == 7)
		{
			if      (0 == std::strncmp(cursor, "CONNECT", 7)) method_ = method::CONNECT;
			else if (0 == std::strncmp(cursor, "OPTIONS", 7)) method_ = method::OPTIONS;
		}

		return end;
	}

	char* Request::parse_uri(char* data)
	{
		// first remove trailing spaces
		char *begin = data;
		while (*begin && *begin == ' ') begin++;

		char* end = begin;
		while(*end && *end != ' ') end++;

		uri_ = std::string(begin, end);

		return end;
	}

	char* Request::parse_headers(char* data)
	{
		char* var = data;

		while(*var)
		{
			char* begin = var;

			// Check where the end of line is. If at the beginning,
			// We are at the end of the headers.
			char* endofline = std::strstr(begin, "\r\n");
			if (!endofline)
			{
				// This is incorrect!
				break;
			}
			else if (endofline == begin)
			{
				var += 2;
				break;
			}

			char* colon = std::strstr(begin, ":");

			std::string variable = std::string(begin, colon);
			// Move the cursor to the start of the next word
			while (*colon && (*colon == ':' || *colon == ' ' )) colon++;

			std::string value = std::string(colon, endofline);
			headers_.insert({variable, value});
			var = endofline + 2;
		}

		return var;
	}

  void Request::log()
  {
    ESP_LOGI("Request", "Method: %s", Request::method_to_str(method()).c_str());
    ESP_LOGI("Request", "URI   : %s", uri().c_str());
    for(auto &h : headers_)
    {
    	ESP_LOGI("Request", "h: '%s': '%s'", h.first.c_str(), h.second.c_str());
    }
		ESP_LOGI("Request", "body  : %s", body().c_str());
  }

  std::string Request::method_to_str(method::eHTTPMethod_t code)
  {
    switch(code)
    {
      case method::GET:
        return "GET";
      case method::HEAD:
        return "HEAD";
      case method::POST:
        return "POST";
      case method::PUT:
        return "PUT";
      case method::DELETE:
        return "DELETE";
      case method::CONNECT:
        return "CONNECT";
      case method::OPTIONS:
        return "OPTIONS";
      case method::TRACE:
        return "TRACE";
      case method::OTHER:
        return "OTHER";
    }
    return "";
  }

}

