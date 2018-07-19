/*
 * DBTransactionImpl.h
 *
 *  Created on: 18 de jul de 2018
 *      Author: rvelloso
 */

#ifndef DBTRANSACTIONIMPL_H_
#define DBTRANSACTIONIMPL_H_

namespace idx {

class DBTransactionImpl {
public:
	DBTransactionImpl();
	virtual ~DBTransactionImpl();
	virtual void commit() = 0;
	virtual void rollback() = 0;
};

} /* namespace idx */

#endif /* DBTRANSACTIONIMPL_H_ */
