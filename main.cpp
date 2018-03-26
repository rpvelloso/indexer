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
	if (!std::experimental::filesystem::exists(dbName))
		ddl = true;

	idx::DB db(dbName, ddl);
	idx::Index idx(db);

	idx::HTTPServer server;

	server.registerService("POST", "/index", [&idx](idx::HTTPClient context, json &input, std::ostream& outp){
		json result;
		std::vector<int> idList;
		int rc = 0;

		try {
			auto transaction = idx.getDB().startTransaction();

			for (auto &i:input["Perguntas"]) {
				std::string uri = i["Resposta"];
				std::string document = i["Pergunta"];

				auto id = idx.index(uri, document);
				if (id > 0)
					idList.push_back(id);
				else
					idList.push_back(-1);
			}

			transaction.commit();
		} catch (...) {
			rc = -1;
		}

		result["ReturnCode"] = rc;
		result["IDs"] = idList;
		context.setResponse(result.dump());
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
				json j;
				if (res.first > 0)
					j = json{ {"id", res.first}, {"score", res.second} };
				else
					j = json{ {"id", -1}, {"score", -1} };

				result["results"].push_back(j);
			}
		} catch (...) {
			rc = -1;
		}
		result["ReturnCode"] = rc;
		context.setResponse(result.dump());
		context.outputResponse(outp);
	});

	server.start();
	return 0;
}
