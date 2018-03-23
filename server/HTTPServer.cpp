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

HTTPServer::HTTPServer() : server(makeServer()) {
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
	registerService("/index", [](json &input, std::ostream& outp){
		int j = 0;
		for (auto &i:input["Perguntas"]) {
			std::cout << ++j << ") " << i["Pergunta"] << " = " << i["Resposta"] << std::endl;
		}
	});

	registerService("/query", [](json &input, std::ostream& outp){
		std::cout << input["Pergunta"] << std::endl;
	});

	server.listen("0.0.0.0", "10000");
}

void HTTPServer::processRequest(HTTPClient& context, std::ostream& outp) {
	outp << "*** requisicao recebida!" << std::endl;
	context.printRequest();
	context.parseHeaders(outp);
}

void HTTPServer::registerService(const std::string& uri,
		ServiceFunction service) {
	services[uri] = service;
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
					context.appendBody(line);
			} else {
				if (line.empty())
					context.setReadingBody(true);
				else {
					context.addHeader(line);
				}
			}
		} else {
			processRequest(context, outp);
			break;
		}
	}
}

void HTTPServer::onConnect(socks::Context<HTTPClient> &ctx, std::istream &inp, std::ostream &outp) {
	HTTPClient &context = ctx.getContext();

	context.setServices(services);
}

} /* namespace idx */
