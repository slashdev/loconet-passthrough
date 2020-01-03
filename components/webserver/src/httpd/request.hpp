#ifndef HTTPDMESSAGE_HPP_
#define HTTPDMESSAGE_HPP_

/*
 *
 * This is the super class used for Request en Response
 * It provides basic functionality to set and read headers
 */

#include <string>
#include <unordered_map>

#include "method.hpp"

#include "baseclass.hpp"

namespace httpd
{

	class Request : public BaseClass
	{

	public:
		Request(char*);
		~Request();

		method::eHTTPMethod_t method();
		std::string uri();

		std::string header(std::string);

		std::string body();

		void log();

		static std::string method_to_str(method::eHTTPMethod_t);
	protected:
		method::eHTTPMethod_t method_ = method::OTHER;
		std::string uri_;
		std::string body_;

		std::unordered_map<std::string, std::string> headers_;
		char* parse_method(char*);
		char* parse_uri(char*);
		char* parse_headers(char*);

	};
}


#endif
