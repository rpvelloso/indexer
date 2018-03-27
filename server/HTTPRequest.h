/*
 * HTTPRequest.h
 *
 *  Created on: 27 de mar de 2018
 *      Author: rvelloso
 */

#ifndef SERVER_HTTPREQUEST_H_
#define SERVER_HTTPREQUEST_H_

#include <vector>
#include <string>

#include "Services.h"

namespace idx {

class HTTPRequest {
public:
	HTTPRequest();
	void addHeader(const std::string &hdr);
	void appendBody(const std::string &b);
	void parseHeaders();
	void printRequest() const;
	bool validURI();

	Method getMethod() const;
	const std::string &getURI() const;
	const std::string &getHTTPVersion() const;
	size_t getContentLength() const;
	const std::string &getBody() const;
private:
	std::vector<std::string> headers;
	std::string httpVersion;
	std::string uri;
	Method method = Method::Unknown;
	std::string body;
	size_t contentLength = 0;
};

} /* namespace idx */

#endif /* SERVER_HTTPREQUEST_H_ */
