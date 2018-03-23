/*
 * DB.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include "DB.h"

namespace idx {

DB::DB(const std::string &dbName, bool ddl) :
	db(dbName),
	dbName(dbName) {
	if (ddl)
		DDL();
}

DBTransaction DB::startTransaction() {
	return db.startTransaction();
}

void DB::DDL() {
	db.createQuery(R"*(
		create table document (
		  id integer primary key,
		  uri varchar
		);)*").execute();

	db.createQuery(R"*(
		create table word (
		  id integer primary key,
		  word varchar unique not null,
		  df integer not null default 0
		);)*").execute();

	db.createQuery(R"*(	
		create table wordcount (
		  document integer not null,
		  word integer not null,
		  count integer default 0,
		  primary key(document, word),
		  foreign key(word) references word(id),
		  foreign key(document) references document(id)
		);)*").execute();

	db.createQuery("create index reverseWCPK on wordcount (word, document);").execute();
	db.createQuery("create index documentURI on document (uri);").execute();
}


} /* namespace idx */
