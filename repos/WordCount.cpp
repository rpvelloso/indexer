/*
 * WordCount.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include "WordCount.h"

namespace idx {

static std::string comma = ",";
static std::string noop = "";

WordCount::WordCount(DB &db) : db(db) {

};

void WordCount::insert(const std::vector<std::tuple<int, int, int> > &records) { // <doc, word, count>
	if (records.size() > 0) {
		auto delim = noop;
		std::string insertSQL = "insert into wordcount (document, word, count) values ";
		for (auto &rec:records) {
			insertSQL += delim + "(" +
				std::to_string(std::get<0>(rec)) + "," +
				std::to_string(std::get<1>(rec)) + "," +
				std::to_string(std::get<2>(rec)) + ")";
			delim = comma;
		}
		db.impl.createQuery(insertSQL).execute();
	}
};

std::vector<std::tuple<int, int, int> > WordCount::list(int docId) {
	auto resultList = db.impl.createQuery(R"*(
		select w.id, wc.count, w.df 
		from wordcount wc, word w 
		where wc.word     = w.id 
		  and wc.document = ?)*",
	docId).getResult();

	std::vector<std::tuple<int, int, int> > result;
	while (resultList.next()) {
		int id, count, df;

		resultList.fetch(id, count, df);
		result.emplace_back(std::forward_as_tuple(id, count, df));
	}
	return result;
}

std::vector<int> WordCount::listDocuments(
		const std::vector<int>& wordIdList) {

	std::string selectSQL;
	auto delim = noop;
	for (auto wid:wordIdList) {
		selectSQL += delim + std::to_string(wid);
		delim = comma;
	}

	auto resultList = db.impl.createQuery(
			"select distinct document from wordcount where word in (" + selectSQL + ");").getResult();

	std::vector<int> result;
	while (resultList.next()) {
		int docId;

		resultList.fetch(docId);
		result.emplace_back(docId);
	}
	return result;
}

} /* namespace idx */
