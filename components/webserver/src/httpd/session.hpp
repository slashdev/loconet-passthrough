#ifndef HTTPSESSION_HPP_
#define HTTPSESSION_HPP_

#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

class HttpSession
{
public:
	HttpSession(int, sockaddr_in);
	~HttpSession();

	void process();

private:
	int socket_ = -1;
	sockaddr_in from_ = {};

	const char* TAG = "HTTPD Session";
};


#endif
