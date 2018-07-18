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

#include "DB.h"
#include "db/transaction/SQLiteTransaction.h"

namespace idx {

DB::DB(const std::string &dbName, bool ddl) :
	impl(dbName),
	dbName(dbName) {
	if (ddl)
		DDL();
}

DBTransaction DB::startTransaction() {
	return DBTransaction(new SQLiteTransaction(impl.startTransaction()));
}

void DB::DDL() {
	impl.createQuery(R"*(
		create table document (
		  id integer primary key,
		  uri varchar
		);)*").execute();

	impl.createQuery(R"*(
		create table word (
		  id integer primary key,
		  word varchar unique not null,
		  df integer not null default 0
		);)*").execute();

	impl.createQuery(R"*(	
		create table wordcount (
		  document integer not null,
		  word integer not null,
		  count integer default 0,
		  primary key(document, word),
		  foreign key(word) references word(id),
		  foreign key(document) references document(id)
		);)*").execute();

	impl.createQuery("create index reverseWCPK on wordcount (word, document);").execute();
	impl.createQuery("create index documentURI on document (uri);").execute();
}


} /* namespace idx */
