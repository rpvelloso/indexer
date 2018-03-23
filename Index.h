/*
 * Index.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#ifndef INDEX_H_
#define INDEX_H_

#include <unordered_map>
#include <unordered_set>

#include "db/DB.h"

namespace idx {

class Index {
public:
	Index(idx::DB &db);
	~Index();
	DB &getDB();
	std::string getDocumentURI(int docId);
	int index(
		const std::string &uri,
		std::string &document);
	std::vector<std::pair<double, int> > query(std::string &queryStr);

private:
	DB &db;
	std::string dbName;
	static std::unordered_set<std::string> stopWords;
	static std::unordered_map<char, char> accents;

	bool processWord(std::string &word);
	void preProcessString(std::string &str);
	std::unordered_map<int, double> queryVector(size_t n, const std::vector<int> &wordIdList);
	std::unordered_map<int, double> documentVector(size_t n, int docId);
	double cosineDistance(
		const std::unordered_map<int, double> &a,
		const std::unordered_map<int, double> &b);
};

} /* namespace idx */

#endif /* INDEX_H_ */
