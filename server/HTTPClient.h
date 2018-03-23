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
private:
	Services *services;
	std::vector<std::string> headers;
	std::string body;
	bool readingBody = false;
	std::string method;
	std::string uri;

	bool validURI();
};

} /* namespace idx */

#endif /* HTTPCLIENT_H_ */
