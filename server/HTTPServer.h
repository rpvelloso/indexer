/*
 * HTTPServer.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <functional>
#include <unordered_map>

#include "libsockets-no-ssl.h"

#include "HTTPClient.h"
#include "Services.h"

namespace idx {

class HTTPServer {
public:
	HTTPServer();
	void start();

	void registerService(const std::string &uri, ServiceFunction service);
private:
	socks::Server server;
	Services services;

	void onReceive(socks::Context<HTTPClient> &ctx, std::istream &inp, std::ostream &outp);
	void onConnect(socks::Context<HTTPClient> &ctx, std::istream &inp, std::ostream &outp);
	static bool readline(std::istream &inp, std::string &line);
	void processRequest(HTTPClient &context, std::ostream &outp);
	socks::Server makeServer();
};

} /* namespace idx */

#endif /* HTTPSERVER_H_ */
