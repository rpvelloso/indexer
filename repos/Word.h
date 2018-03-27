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

#ifndef WORD_H_
#define WORD_H_

#include "db/DB.h"

namespace idx {

class DB;

class Word {
public:
	Word(DB &db, int id);
	Word(DB &db, const std::string &word);
	Word(DB &db);
	void update();
	const std::string &getWord() const;
	int getID() const;
	int getDF() const;
	void setDF(int df);
	std::vector<std::pair<int, int> > list(const std::vector<int> &wordIdList);
private:
	int id;
	std::string word;
	int df;
	DB &db;

	void fetch(int id);
	void fetch(const std::string &word);
};

} /* namespace idx */

#endif /* WORD_H_ */
