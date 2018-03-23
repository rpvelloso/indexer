#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

#include "server/HTTPServer.h"
#include "db/DB.h"
#include "Index.h"

std::string comma = ",";
std::string noop = "";

class FileLoader {
private:
	using LoadedDataType = std::vector<std::vector<std::string> >;
	static const char delim = '^';
	std::string filename;
	std::vector<std::string> fields;
	LoadedDataType data;
public:
	FileLoader(const std::string &filename, const std::vector<std::string> &fields) : filename(filename), fields(fields) {
		load();
	};
	
	void load() {
		data.clear();
		
		std::fstream file(filename);
		std::string line;

		std::getline(file, line);
		while (file) {
			if (!line.empty() && line[0] != '#') {
				std::stringstream ss(line + delim);
				std::string fvalue;
				
				int field = 0;
				std::getline(ss, fvalue, delim);
				data.emplace_back(std::vector<std::string>(fields.size()));
				while (ss) {
					data.back()[field++] = fvalue;
					std::getline(ss, fvalue, delim);
				}
			}
			std::getline(file, line);
		}

		std::cout << "file " << filename << " loaded" << std::endl;
	};
	
	virtual ~FileLoader() {};
	
	LoadedDataType &getData() {return data;};
};

int main (int argc, char **argv) {
	idx::HTTPServer server;

	server.start();
	return 0;

	std::string dbName = "tmp.db";
	
	if (argc == 2 && std::string(argv[1]) == "index") {
		FileLoader fileLoader("inp.txt",{"resposta", "pergunta"});

		if (dbName != ":memory:")
			remove(dbName.c_str());
		idx::DB db(dbName, true);
		idx::Index idx(db);
		
		auto data = fileLoader.getData();
		auto transaction = idx.getDB().startTransaction();
		for (auto &rec:data) {
			std::unordered_map<std::string, size_t> wordCount;
			std::string input(rec[1]);
			
			std::cout << "indexing document " << rec[0] << " ... ";
			idx.index(rec[0], input);
			std::cout << "done." << std::endl;
		}
		transaction.commit();

	} else if (argc == 3 && std::string(argv[1]) == "query") {
		idx::DB db(dbName);
		idx::Index idx(db);
		std::string query(argv[2]);
		
		auto res = idx.query(query);
		for (auto &i:res) {
			auto docId = i.second;
			auto uri = idx.getDocumentURI(docId); // get label
			std::cout << i.first << " = " << uri << std::endl;
		}
	}/* else if (argc == 2 && std::string(argv[1]) == "dump") {
		idx::Index idx(dbName);
		auto result = idx.getDB().createQuery(R"*(
			select w.word, w.df, sum(wc.count) count
			from word w, wordcount wc
			where w.id = wc.word
			group by w.id
			order by df desc, count
			
			
		)*").getResult();
		while (result.next()) {
			std::string word;
			int df, count;
			
			result.fetch(word, df, count);
			std::cout << word << "\t" << df << "\t" << count << std::endl;
		}
	}*/
}
