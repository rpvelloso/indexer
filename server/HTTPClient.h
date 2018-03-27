/*
    Copyright 2018 Roberto Panerai Velloso.
    This file is part of libjukebox.
    libjukebox is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    libjukebox is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with libjukebox.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HTTPCLIENT_H_
#define HTTPCLIENT_H_

#include <vector>
#include <string>

#include "Services.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"

namespace idx {

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
