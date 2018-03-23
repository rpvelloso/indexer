/*
 * DB.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#ifndef DB_H_
#define DB_H_

#include "sqlite.c++.h"

#include "repos/Document.h"
#include "repos/Word.h"
#include "repos/WordCount.h"

#include "DBTransaction.h"

namespace idx {

class DB {
friend class Document;
friend class Word;
friend class WordCount;

public:
	DB(const std::string &dbName, bool ddl = false);
	DBTransaction startTransaction();
private:
	SQLiteDB db;
	std::string dbName;
	void DDL();
};

} /* namespace idx */

#endif /* DB_H_ */
