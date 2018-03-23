/*
 * Word.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
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
