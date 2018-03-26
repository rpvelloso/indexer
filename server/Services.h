/*
 * Services.h
 *
 *  Created on: 22 de mar de 2018
 *      Author: rvelloso
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include <vector>
#include <functional>
#include <unordered_map>


#include "json.hpp"

using json = nlohmann::json;

namespace idx {

enum class Method {
	Unknown = -1,
	GET = 0,
	POST = 1
};

inline Method methodStr2Enum(const std::string& methodStr) {
	if (methodStr == "GET")
		return Method::GET;
	if (methodStr == "POST")
		return Method::POST;

	return Method::Unknown;
}

class HTTPClient;

using ServiceFunction = std::function<void(idx::HTTPClient &context, json &input, std::ostream& outp)>;
using Services = std::vector<std::unordered_map<std::string, ServiceFunction> >;

}
#endif /* SERVICES_H_ */
