/*
 * Services.h
 *
 *  Created on: 22 de mar de 2018
 *      Author: rvelloso
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#include <functional>
#include <unordered_map>

#include "json.hpp"

using json = nlohmann::json;
using ServiceFunction = std::function<void(json &input, std::ostream& outp)>;
using Services = std::unordered_map<std::string, ServiceFunction>;

#endif /* SERVICES_H_ */
