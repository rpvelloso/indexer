/*
 * DBTransaction.cpp
 *
 *  Created on: 21 de mar de 2018
 *      Author: rvelloso
 */

#include "DBTransaction.h"

namespace idx {

DBTransaction::DBTransaction(SQLiteTransationGuard tr) : tr(std::move(tr)) {
}

void DBTransaction::commit() {
	tr.commit();
}

} /* namespace idx */
