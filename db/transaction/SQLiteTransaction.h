/*
 * SQLiteTransation.h
 *
 *  Created on: 18 de jul de 2018
 *      Author: rvelloso
 */

#ifndef SQLITETRANSACTION_H_
#define SQLITETRANSACTION_H_

#include "db/transaction/DBTransactionImpl.h"
#include "db/sqlite.c++.h"

namespace idx {

class SQLiteTransaction: public DBTransactionImpl {
public:
	SQLiteTransaction(SQLiteTransactionGuard tr);
	virtual ~SQLiteTransaction();
	virtual void commit() override;
private:
	SQLiteTransactionGuard tr;
};

} /* namespace idx */

#endif /* SQLITETRANSACTION_H_ */
