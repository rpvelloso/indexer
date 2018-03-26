/*
 * HTTPClient.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include <iostream>

#include "server/HTTPClient.h"
#include "server/json.hpp"

static std::unordered_map<int, std::string> replyStr = {
	{200, "OK"},
	{404, "Not Found."},
	{500, "Internal Server Error."}
};


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
	reply = HTTPReply::NOTFOUND;

	if (headers.size() > 0) {
		std::string methodStr;
		std::stringstream ss(headers[0]);

		ss >> methodStr;
		ss >> uri;
		ss >> httpVersion;

		method = methodStr2Enum(methodStr);


		if (method != Method::Unknown && validURI()) {
			if (services != nullptr) {
				auto serviceIt = (*services)[(int)method].find(uri);
				if (serviceIt != (*services)[(int)method].end()) {
					auto service = serviceIt->second;

					try {
						reply = HTTPReply::OK;
						json input = json::parse(body);

						service(*this, input, outp);
						return;
					} catch (...) {
						reply = HTTPReply::INTERNALSERVERERROR;
					}
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
void HTTPClient::outputResponse(std::ostream& outp) {
	outp << httpVersion << " " << (int)reply << " " << replyStr[(int)reply] << CRLF;
	outp << "Content-length: " << response.size() << CRLF;
	outp << "Content-type: application/json" << CRLF;
	outp << "Connection: close" << CRLF;
	outp << CRLF;
	outp << response << std::endl;
}

void HTTPClient::setResponse(const std::string& resp) {
	response = resp;
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
