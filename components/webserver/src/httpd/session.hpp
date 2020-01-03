#ifndef HTTPSESSION_HPP_
#define HTTPSESSION_HPP_

#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

#include "request.hpp"
#include "response.hpp"
#include "baseclass.hpp"

#ifndef HTTD_SERVER_MAX_REQUEST_LENGTH
#define HTTD_SERVER_MAX_REQUEST_LENGTH 512
#endif

namespace httpd
{

	class Session : public BaseClass
	{
	public:
		Session(int, sockaddr_in);
		~Session();

		Request* request();
		Response* response();

		void reply();

	private:
		void process();

		int socket_ = -1;
		sockaddr_in from_ = {};

		Request* request_;
		Response* response_;

		const char* TAG = "HTTPD Session";
	};

}

#endif
