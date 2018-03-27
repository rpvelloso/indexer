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
	SQLiteDB impl;
	std::string dbName;
	void DDL();
};

} /* namespace idx */

#endif /* DB_H_ */
