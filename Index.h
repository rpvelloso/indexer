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
	std::vector<std::pair<int, double> > query(std::string &queryStr);

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
