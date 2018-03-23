/*
 * Document.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include "Document.h"

namespace idx {

Document::Document(DB &db, int id) : db(db) {
	fetch(id);
};

Document::Document(DB &db, const std::string &uri) : db(db) {
	fetch(uri);
};

Document::Document(DB &db) : db(db) {

};

void Document::fetch(int id) {
	auto result = db.impl.createQuery("select id, uri from document where id = ?", id).getResult();
	if (result.next()) {
		result.fetch(
			this->id,
			this->uri);
	}
};
void Document::fetch(const std::string &uri) {
	/*auto result = db.db.createQuery("select id, uri from document where uri = ?", uri).getResult();
	if (result.next()) {
		result.fetch(
			this->id,
			this->uri);
	} else {*/
		db.impl.createQuery("insert into document (uri) values (?);", uri).execute();
		this->id = db.impl.lastInsertRowID();
		this->uri = uri;
	//}
};

void Document::clean() {
	db.impl.createQuery(R"*(
		update word set df=df-1 where id in (
		select word from wordcount where document = ?);
	)*", id).execute();

	db.impl.createQuery("delete from wordcount where document = ?;", id).execute();
};

const std::string &Document::getURI() const {
	return uri;
};

int Document::getID() const {
	return id;
};

int Document::count() {
	int n;
	auto result = db.impl.createQuery("select count(*) from document;").getResult();
	result.next();
	result.fetch(n);
	return n;
};

} /* namespace idx */
