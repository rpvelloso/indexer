/*
 * DBTransaction.h
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#ifndef DBTRANSACTION_H_
#define DBTRANSACTION_H_

#include "sqlite.c++.h"

namespace idx {

class DBTransaction {
public:
	DBTransaction(SQLiteTransationGuard tr);
	void commit();
private:
	SQLiteTransationGuard tr;
};

} /* namespace idx */

#endif /* DBTRANSACTION_H_ */
