/*
 * Word.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include "Word.h"

namespace idx {

Word::Word(DB &db, int id) : db(db) {
	fetch(id);
};

Word::Word(DB &db, const std::string &word) : db(db) {
	fetch(word);
};

Word::Word(DB &db) : db(db) {

};

void Word::fetch(int id) {
	auto result = db.db.createQuery("select id, word, df from word where id = ?", id).getResult();
	if (result.next()) {
		result.fetch(
			this->id,
			this->word,
			this->df);
	}
};

void Word::fetch(const std::string &word) {
	auto result = db.db.createQuery("select id, word, df from word where word = ?", word).getResult();
	if (result.next()) {
		result.fetch(
			this->id,
			this->word,
			this->df);
	} else {
		db.db.createQuery("insert into word (word, df) values (?, 0);", word).execute();
		this->id = db.db.lastInsertRowID();
		this->word = word;
		this->df = 0;
	}
};

void Word::update() {
	db.db.createQuery("update word set df = ? where id = ?;", df, id).execute();
}

const std::string &Word::getWord() const {
	return word;
};

int Word::getID() const {
	return id;
};

int Word::getDF() const {
	return df;
};

void Word::setDF(int df) {
	this->df = df;
};

std::vector<std::pair<int, int> > Word::list(const std::vector<int> &wordIdList) {
	std::string comma = ",";
	std::string noop = "";

	std::string selectSQL;
	auto delim = noop;
	for (auto wid:wordIdList) {
		selectSQL += delim + std::to_string(wid);
		delim = comma;
	}
	auto resultList = db.db.createQuery("select id, df from word where id in (" + selectSQL + ");").getResult();
	std::vector<std::pair<int, int> > result;
	while (resultList.next()) {
		int id, df;
		resultList.fetch(id, df);
		result.emplace_back(std::make_pair(id, df));
	}
	return result;
}

} /* namespace idx */

