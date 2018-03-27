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
          and wc.count > 0 
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
