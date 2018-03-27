/*
 * HTTPResponse.cpp
 *
 *  Created on: 27 de mar de 2018
 *      Author: rvelloso
 */

#include <unordered_map>

#include "server/HTTPResponse.h"

namespace idx {

static std::unordered_map<int, std::string> replyStr = {
	{200, "OK"},
	{404, "Not Found."},
	{500, "Internal Server Error."}
};

HTTPResponse::HTTPResponse() {

}

void HTTPResponse::appendBody(const std::string& b) {
	body.append(b.begin(), b.end());
	body.push_back('\n');
}

const std::string& HTTPResponse::getBody() const {
	return body;
}

HTTPReply HTTPResponse::getReply() const {
	return reply;
}

void HTTPResponse::setReply(HTTPReply r) {
	reply = r;
}

void HTTPResponse::buildHeaders() {
	headers.emplace_back(
		httpVersion + " " +
		std::to_string((int)reply) + " " +
		replyStr[(int)reply]);

	headers.emplace_back("Content-length: " + std::to_string(body.size()));
	headers.emplace_back("Content-type: " + contentType);
	headers.emplace_back("Connection: close");
}

const std::vector<std::string>& HTTPResponse::getHeaders() const {
	return headers;
}

const std::string& HTTPResponse::getHTTPVersion() const {
	return httpVersion;
}

void HTTPResponse::setHTTPVersion(const std::string& httpVersion) {
	this->httpVersion = httpVersion;
}

const std::string& HTTPResponse::getContentType() const {
	return contentType;
}

void HTTPResponse::setContentType(const std::string& contentType) {
	this->contentType = contentType;
}

} /* namespace idx */
