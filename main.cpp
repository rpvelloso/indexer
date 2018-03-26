#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <experimental/filesystem>

#include "server/HTTPServer.h"
#include "db/DB.h"

#include "Index.h"

int main (int argc, char **argv) {
	std::string dbName = "tmp.db";
	bool ddl = false;
	idx::HTTPServer server;
	if (!std::experimental::filesystem::exists(dbName))
		ddl = true;

	idx::DB db(dbName, ddl);
	idx::Index idx(db);

	server.registerService("POST", "/index", [&idx](idx::HTTPClient context, json &input, std::ostream& outp){
		json result;
		std::vector<int> idList;
		int rc = 0;

		try {
			for (auto &i:input["Perguntas"]) {
				std::string uri = i["Resposta"];
				std::string document = i["Pergunta"];

				auto id = idx.index(uri, document);
				idList.push_back(id);
			}
		} catch (...) {
			rc = -1;
		}

		result["rc"] = rc;
		result["IDs"] = idList;
		context.setResponse(result.dump(4));
		context.outputResponse(outp);
	});

	server.registerService("POST", "/query", [&idx](idx::HTTPClient context, json &input, std::ostream& outp){
		json result;
		int rc = 0;
		std::vector<std::pair<int, double> > queryResults;
		try {
			result["results"] = json::array();
			std::string query = input["Pergunta"];

			queryResults = idx.query(query);
			for (auto &res:queryResults) {
				json j = json{ {"id", res.first}, {"score", res.second} };
				result["results"].push_back(j);
			}
		} catch (...) {
			rc = -1;
		}
		result["rc"] = rc;
		context.setResponse(result.dump(4));
		context.outputResponse(outp);
	});

	server.start();
	return 0;
}
