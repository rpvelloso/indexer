/*
 * HTTPClient.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <vector>
#include <string>

#include "Services.h"

namespace idx {

enum class HTTPReply {
	OK = 200,
	NOTFOUND = 404,
	INTERNALSERVERERROR = 500
};

class HTTPClient {
public:
	HTTPClient();
	void addHeader(const std::string &hdr);
	bool isReadingBody();
	void setReadingBody(bool r);
	void appendBody(const std::string &b);
	const std::string &getBody() const;
	void parseHeaders(std::ostream& outp);

	void printRequest() const;
	void setServices(Services &services);
	void setResponse(const std::string &resp);
	void outputResponse(std::ostream& outp);
private:
	Services *services;
	std::vector<std::string> headers;
	std::string httpVersion;
	std::string body;
	std::string response;
	bool readingBody = false;
	Method method;
	std::string uri;
	HTTPReply reply = HTTPReply::OK;

	bool validURI();
};

} /* namespace idx */

#endif /* HTTPCLIENT_H_ */
