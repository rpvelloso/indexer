/*
 * SQLiteTransation.cpp
 *
 *  Created on: 18 de jul de 2018
 *      Author: rvelloso
 */

#include <db/transaction/SQLiteTransaction.h>

namespace idx {

SQLiteTransaction::SQLiteTransaction(SQLiteTransactionGuard tr) : tr(std::move(tr)) {
}

SQLiteTransaction::~SQLiteTransaction() {
}

void SQLiteTransaction::commit() {
	tr.commit();
}

void SQLiteTransaction::rollback() {
	tr.rollback();
}

} /* namespace idx */
