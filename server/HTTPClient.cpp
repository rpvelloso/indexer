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

void HTTPClient::addHeader(const std::string& hdr) {
	headers.emplace_back(hdr);
}

bool HTTPClient::isReadingBody() {
	return readingBody;
}

void HTTPClient::setReadingBody(bool r) {
	readingBody = r;
}

void HTTPClient::appendBody(const std::string& b) {
	body.append(b.begin(), b.end());
	body.push_back('\n');
}

const std::string& HTTPClient::getBody() const {
	return body;
}

void HTTPClient::parseHeaders(std::ostream& outp) {
	if (headers.size() > 0) {
		std::stringstream ss(headers[0]);

		ss >> method;
		ss >> uri;

		if (method == "POST" && validURI()) {
			if (services != nullptr) {
				auto serviceIt = services->find(uri);
				if (serviceIt != services->end()) {
					auto service = serviceIt->second;

					json input = json::parse(body);

					service(input, outp);
				}
			}
		}
	}
}

void HTTPClient::setServices(Services& services) {
	this->services = &services;
}

bool HTTPClient::validURI() { /* add more validation */
	return !uri.empty();
}

void HTTPClient::printRequest() const {
	std::cout << "*** headers: " << std::endl;
	for (auto &h:headers)
		std::cout << h << std::endl;
	std::cout << "*** body: " << std::endl;
	std::cout << body << std::endl;
}

} /* namespace idx */
