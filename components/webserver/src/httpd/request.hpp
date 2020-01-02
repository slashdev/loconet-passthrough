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

namespace httpd
{

	class Request
	{

	public:
		Request(char*, size_t);
		~Request();

		method::eHTTPMethod_t method();
		std::string uri();

		std::string header(std::string);

		std::string body();
		std::string variable(std::string);

	protected:
		method::eHTTPMethod_t method_;
		std::string uri_;
		std::string body_;

		std::unordered_map<std::string, std::string> headers_;
//		std::unordered_map<std::string, std::string> variables_;
		void parse_headers();

		char* parse_method(char*);
		char* parse_uri(char*);
	};
}


#endif
