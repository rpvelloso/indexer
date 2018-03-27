/*
 * HTTPClient.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include <iostream>

#include "server/HTTPClient.h"
#include "server/json.hpp"

using json = nlohmann::json;

namespace idx {

HTTPClient::HTTPClient() {
}

bool HTTPClient::isReadingBody() {
	return readingBody;
}

void HTTPClient::setReadingBody(bool r) {
	readingBody = r;
}

void HTTPClient::processRequest(std::ostream& outp) {
	response.setReply(HTTPReply::NotFound);

	request.parseHeaders();
	response.setHTTPVersion(request.getHTTPVersion());

	auto method = request.getMethod();
	if (method != Method::Unknown && request.validURI()) {
		if (services != nullptr) {
			auto serviceIt = (*services)[(int)method].find(request.getURI());
			if (serviceIt != (*services)[(int)method].end()) {
				auto service = serviceIt->second;

				try {
					json input = json::parse(request.getBody());

					std::cerr << ">>> INPUT: " << std::endl << input.dump(2) << std::endl;

					service(*this, input);
				} catch (...) {
					response.setReply(HTTPReply::InternalServerError);
				}
			}
		}
	}

	outputResponse(outp);
}

void HTTPClient::setServices(Services& services) {
	this->services = &services;
}

#define CRLF "\r\n"

HTTPRequest& HTTPClient::getRequest() {
	return request;
}

HTTPResponse& HTTPClient::getResponse() {
	return response;
}

void HTTPClient::outputResponse(std::ostream& outp) {
	response.buildHeaders();
	for (auto &hdr:response.getHeaders())
		outp << hdr << CRLF;
	outp << CRLF;
	outp << response.getBody() << std::endl;

	std::cerr << "<<< OUTPUT:" << std::endl << response.getBody() << std::endl;
}

} /* namespace idx */
