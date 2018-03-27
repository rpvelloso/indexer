/*
 * HTTPServer.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include <functional>

#include "server/HTTPServer.h"
#include "server/HTTPClient.h"

namespace idx {

HTTPServer::HTTPServer() : server(makeServer()), services(2) {
}

bool HTTPServer::readline(std::istream &inp, std::string &line) {

	auto savePos = inp.tellg();
	std::getline(inp, line);
	//std::cout << (bool)inp << " " << inp.eof() << "" << line << std::endl;
	if (inp)
		return true;
	else
		inp.seekg(savePos);

	return false;
}

void HTTPServer::start() {
	server.listen("0.0.0.0", "10000");
}

void HTTPServer::registerService(const std::string &methodStr, const std::string& uri,
		ServiceFunction service) {
	auto method = methodStr2Enum(methodStr);
	if (method != Method::Unknown)
		services[(int)method][uri] = service;
	else
		throw std::runtime_error("Unkown method " + methodStr);
}

socks::Server HTTPServer::makeServer() {
	return socks::factory::makeThreadedServer<HTTPClient>(
		std::bind(&HTTPServer::onReceive, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3),
		std::bind(&HTTPServer::onConnect, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3));
}

void HTTPServer::onReceive(socks::Context<HTTPClient> &ctx, std::istream &inp, std::ostream &outp) {
	HTTPClient &context = ctx.getContext();

	while (inp) {
		std::string line;

		if (readline(inp, line)) {
			if (line.back() == '\r')
				line.pop_back();

			if (context.isReadingBody()) {
				if (!line.empty())
					context.getRequest().appendBody(line);
			} else {
				if (line.empty())
					context.setReadingBody(true);
				else {
					context.getRequest().addHeader(line);
				}
			}
		} else {
			context.processRequest(outp);
			break;
		}
	}
}

void HTTPServer::onConnect(socks::Context<HTTPClient> &ctx, std::istream &inp, std::ostream &outp) {
	HTTPClient &context = ctx.getContext();

	context.setServices(services);
}

} /* namespace idx */
