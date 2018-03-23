/*
 * WordCount.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#ifndef WORDCOUNT_H_
#define WORDCOUNT_H_

#include <vector>
#include <tuple>

#include "db/DB.h"

namespace idx {

class DB;

class WordCount {
public:
	WordCount(DB &db);
	void insert(const std::vector<std::tuple<int, int, int> > &records);
	std::vector<std::tuple<int, int, int> > list(int docId);
	std::vector<int> listDocuments(const std::vector<int> &wordIdList);
private:
	DB &db;
};

} /* namespace idx */

#endif /* WORDCOUNT_H_ */
