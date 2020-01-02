#include "request.hpp"

#include <cstring>

#include "esp_log.h"

namespace httpd
{
	Request::~Request()
	{
		headers_.clear();
	}

	Request::Request(char *data, size_t length)
	{
		char *cursor = data;
		if (length == 0 ) {
			// This should not happen! Do some initialization
			method_ = method::HEAD;
			uri_ = "/";
			body_ = "";

			return;
		}

		// parse the method
		cursor = parse_method(cursor);

		ESP_LOGI("Request", "I have method: '%s'", std::string(data, cursor).c_str());

		cursor = parse_uri(cursor);

		ESP_LOGI("Request", "I have uri: %s", uri_.c_str());

		// do the rest


	}

	method::eHTTPMethod_t Request::method()
	{
		return method_;
	}

	std::string Request::uri()
	{
		return "";
	}

	std::string Request::header(std::string)
	{
		return "";
	}

	std::string Request::body()
	{
		return "";
	}

	char* Request::parse_method(char* cursor)
	{
		switch(cursor[0])
		{
			case 'G':
				method_ = method::GET;
				return (cursor + 3);
			case 'H':
				method_ = method::HEAD;
				return (cursor + 4);
			case 'P':
				if (cursor[1] == 'U' )
				{
					method_ = method::PUT;
					return (cursor + 3);
				}
				else
				{
					method_ = method::POST;
					return (cursor + 4);
				}
			case 'D':
				method_ = method::DELETE;
				return (cursor + 6);
			case 'C':
				method_ = method::CONNECT;
				return (cursor + 7);
			case 'O':
				method_ = method::OPTIONS;
				return (cursor + 7);
			case 'T':
				method_ = method::TRACE;
				return (cursor + 5);
			default:
				return cursor;
		}
	}

	char* Request::parse_uri(char* data)
	{
		// first remove trailing spaces
		char *begin = data;
		while (*begin && *begin == ' ')
		{
			begin++;
		}
		char* end = begin;
		while(*end && *end != ' ')
		{
			end++;
		}
		uri_ = std::string(begin, end);

		return end;
	}
}

