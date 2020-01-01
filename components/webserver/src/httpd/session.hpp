#ifndef HTTPSESSION_HPP_
#define HTTPSESSION_HPP_

#include "tcpip_adapter.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"

namespace communication
{
  namespace httpd
  {

		class Session
		{
		public:
			Session(int, sockaddr_in);
			~Session();

			void process();

		private:
			int socket_ = -1;
			sockaddr_in from_ = {};

			const char* TAG = "HTTPD Session";
		};

	}
}

#endif
