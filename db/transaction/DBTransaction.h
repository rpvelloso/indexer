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

#ifndef DBTRANSACTION_H_
#define DBTRANSACTION_H_

#include "db/sqlite.c++.h"
#include "db/transaction/DBTransactionImpl.h"

namespace idx {

class DBTransaction {
public:
	DBTransaction(DBTransactionImpl *impl);
	void commit();
private:
	std::unique_ptr<DBTransactionImpl> impl;
};

} /* namespace idx */

#endif /* DBTRANSACTION_H_ */
