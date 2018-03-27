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
#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace idx {

//TODO: create request & response objects, pass them to the service callback

class HTTPClient {
public:
	HTTPClient();
	bool isReadingBody();
	void setReadingBody(bool r);
	HTTPRequest &getRequest();
	HTTPResponse &getResponse();
	void processRequest(std::ostream& outp);
	void setServices(Services &services);

private:
	Services *services = nullptr;
	HTTPRequest request;
	HTTPResponse response;
	bool readingBody = false;

	void outputResponse(std::ostream& outp);
};

} /* namespace idx */

#endif /* HTTPCLIENT_H_ */
