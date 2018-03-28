/*
    Copyright 2018 Roberto Panerai Velloso.
    This file is part of indexer.
    Indexer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Indexer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Indexer.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <experimental/filesystem>
#include <getopt.h>

#include "server/HTTPServer.h"
#include "db/DB.h"

#include "Index.h"

void usage(const std::string &prog) {
	std::cerr << prog << " [-p port] [-v]" << std::endl;
	std::cerr << "-p port number. Default: 10000." << std::endl;
	std::cerr << "-v enables verbose output. Default off." << std::endl;
	std::cerr << "ex.: " << prog << " -p 8080 -v" << std::endl;
}

struct Options {
	std::string port = "10000";
	bool verbose = false;
	bool exit = false;
};

Options parseOptions(int argc, char **argv) {
	Options options;
	int c;
	while ((c = getopt(argc, argv, "vhp:")) != -1) {
		switch (c) {
		case 'p':
			options.port = optarg;
			break;
		case 'v':
			options.verbose = true;
			break;
		case 'h':
		default:
			usage(argv[0]);
			options.exit = true;
		}
	}

	return options;
}

int main (int argc, char **argv) {
	auto options = parseOptions(argc, argv);

	if (options.exit)
		return 0;

	std::string dbName = "tmp.db";
	bool ddl = false;
	if (!std::experimental::filesystem::exists(dbName))
		ddl = true;

	idx::DB db(dbName, ddl);
	idx::Index index(db);

	idx::HTTPServer server(options.verbose);

	server.registerService("POST", "/index", [&index](idx::HTTPClient &context, json &input){
		json result;

		try {
			std::vector<int> idList;
			auto transaction = index.getDB().startTransaction();

			for (auto &i:input["Perguntas"]) {
				std::string uri = i["Resposta"];
				std::string document = i["Pergunta"];

				auto id = index.index(uri, document);
				if (id > 0)
					idList.push_back(id);
				else
					idList.push_back(-1);
			}

			transaction.commit();

			result["ReturnCode"] = 0;
			result["IDs"] = idList;

			context.getResponse().setReply(idx::HTTPReply::OK);
		} catch (...) {
			result["ReturnCode"] = -1;
			context.getResponse().setReply(idx::HTTPReply::InternalServerError);
		}
		context.getResponse().appendBody(result.dump(2));
	});

	server.registerService("POST", "/query", [&index](idx::HTTPClient &context, json &input){
		json result;

		try {
			std::vector<std::pair<int, double> > queryResults;

			result["results"] = json::array();
			std::string query = input["Pergunta"];

			queryResults = index.query(query);
			for (auto &res:queryResults) {
				json j;
				if (res.first > 0)
					j = json{ {"id", res.first}, {"score", res.second} };
				else
					j = json{ {"id", -1}, {"score", -1} };

				result["results"].push_back(j);
			}
			result["ReturnCode"] = 0;
			context.getResponse().setReply(idx::HTTPReply::OK);
		} catch (...) {
			result["ReturnCode"] = -1;
			context.getResponse().setReply(idx::HTTPReply::InternalServerError);
		}
		context.getResponse().appendBody(result.dump(2));
	});

	server.start("0.0.0.0", options.port);
	return 0;
}
