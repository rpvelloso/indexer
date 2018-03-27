/*
 * HTTPRequest.cpp
 *
 *  Created on: 27 de mar de 2018
 *      Author: rvelloso
 */

#include "server/HTTPRequest.h"

namespace idx {

HTTPRequest::HTTPRequest() {
}

void HTTPRequest::addHeader(const std::string& hdr) {
	headers.emplace_back(hdr);
}

void HTTPRequest::appendBody(const std::string& b) {
	body.append(b.begin(), b.end());
	body.push_back('\n');
}

void HTTPRequest::parseHeaders() {
	if (headers.size() > 0) {
		std::string methodStr;
		std::stringstream ss(headers[0]);

		ss >> methodStr;
		ss >> uri;
		ss >> httpVersion;

		method = methodStr2Enum(methodStr);

		/*for (size_t i = 1; i < headers.size(); ++i) {
			//TODO: parse remaining headers (content type, length, etc)
		}*/
	}
}

void HTTPRequest::printRequest() const {
	std::cout << "*** headers: " << std::endl;
	for (auto &h:headers)
		std::cout << h << std::endl;
	std::cout << "*** body: " << std::endl;
	std::cout << body << std::endl;
}


bool HTTPRequest::validURI() { /*TODO: add more validation */
	return !uri.empty();
}

Method HTTPRequest::getMethod() const {
	return method;
}

const std::string& HTTPRequest::getURI() const {
	return uri;
}

const std::string& HTTPRequest::getHTTPVersion() const {
	return httpVersion;
}

size_t HTTPRequest::getContentLength() const {
	return contentLength;
}

const std::string& HTTPRequest::getBody() const {
	return body;
}

} /* namespace idx */
