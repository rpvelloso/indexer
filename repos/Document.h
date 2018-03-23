/*
 * Document.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include "db/DB.h"

namespace idx {

class DB;

class Document {
public:
	Document(DB &db, int id);
	Document(DB &db, const std::string &uri);
	Document(DB &db);
	void clean();
	const std::string &getURI() const;
	int getID() const;
	int count();
private:
	int id;
	std::string uri;
	DB &db;

	void fetch(int id);
	void fetch(const std::string &uri);
};

} /* namespace idx */

#endif /* DOCUMENT_H_ */
