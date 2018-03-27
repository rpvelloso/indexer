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
	auto result = db.impl.createQuery("select id, word, df from word where id = ?", id).getResult();
	if (result.next()) {
		result.fetch(
			this->id,
			this->word,
			this->df);
	}
};

void Word::fetch(const std::string &word) {
	auto result = db.impl.createQuery("select id, word, df from word where word = ?", word).getResult();
	if (result.next()) {
		result.fetch(
			this->id,
			this->word,
			this->df);
	} else {
		db.impl.createQuery("insert into word (word, df) values (?, 0);", word).execute();
		this->id = db.impl.lastInsertRowID();
		this->word = word;
		this->df = 0;
	}
};

void Word::update() {
	db.impl.createQuery("update word set df = ? where id = ?;", df, id).execute();
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
	auto resultList = db.impl.createQuery("select id, df from word "
			"where df > 0 and id in (" + selectSQL + ");").getResult();
	std::vector<std::pair<int, int> > result;
	while (resultList.next()) {
		int id, df;
		resultList.fetch(id, df);
		result.emplace_back(std::make_pair(id, df));
	}
	return result;
}

} /* namespace idx */

