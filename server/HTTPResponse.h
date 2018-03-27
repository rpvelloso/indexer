/*
 * HTTPResponse.h
 *
 *  Created on: 27 de mar de 2018
 *      Author: rvelloso
 */

#ifndef SERVER_HTTPRESPONSE_H_
#define SERVER_HTTPRESPONSE_H_

#include <string>
#include <vector>

namespace idx {

enum class HTTPReply {
	OK = 200,
	NotFound = 404,
	InternalServerError = 500
};

//TODO: build header lines from content variables, content-type, etc
class HTTPResponse {
public:
	HTTPResponse();
	void appendBody(const std::string &b);
	const std::string &getBody() const;
	HTTPReply getReply() const;
	void setReply(HTTPReply r);
	void buildHeaders();
	const std::vector<std::string> &getHeaders() const;
	const std::string& getHTTPVersion() const;
	void setHTTPVersion(const std::string& httpVersion);

private:
	std::string httpVersion;
	HTTPReply reply = HTTPReply::NotFound;
	std::vector<std::string> headers;
	std::string body;
};

} /* namespace idx */

#endif /* SERVER_HTTPRESPONSE_H_ */
