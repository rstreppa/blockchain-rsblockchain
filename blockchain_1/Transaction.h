/**
Transaction.h
Purpose:	Transaction class for nodes

@author rstreppa
@version 1.0 09/12/2018
*/
#pragma once

#include <vector>
#include <string>
#include <boost/random.hpp>

using namespace std;


class Transaction
{
private:
	unsigned int m_ID = 0;		///< sequential ID
	string m_timestamp;			///< boost timestamp
	unsigned short m_from;		///< port number, for coinbase transaction port is 0
	unsigned short m_to;		///< port number, for coinbase transaction port is node port 
	double m_amount;			///< signed amount that will add to Node::m_balance
public:
	/**	@brief	Construct a Transaction given from/to ports and amount */
	Transaction(unsigned short from, unsigned short to, double amount);
	/**	@brief	Construct a Transaction given from/to ports and random amount */
	Transaction(unsigned short from, unsigned short to, boost::random::mt19937_64& rng);
	/**	@brief	Default C'tor, necessary to compile Block C'tor*/
	Transaction() {}
	/**	@brief	Construct a Transaction given all the fields, necessary for jsonification */
	Transaction(unsigned int ID, string timestamp, unsigned short from, unsigned short to, double amount);
	/**	@brief	Copy Constructor */
	Transaction(const Transaction&) = default;
	/**	@brief	Assignement Operator */
	Transaction& operator=(const Transaction& other);
	
	/** @brief convert transaction to Json */
	string toJson();

	/**	@brief access Transaction ID */
	int getID()	const				{ return(m_ID); }
	/**	@brief access Transaction timestamp */
	string getTimestamp() const		{ return(m_timestamp); }
	/**	@brief access Transaction from port */
	unsigned short getFrom() const	{ return(m_from); }
	/**	@brief access Transaction to port */
	unsigned short getTo() const	{ return(m_to); }
	/**	@brief access Transaction amount */
	double getAmount() const		{ return(m_amount); }
	/** @brief comparison operators */
	friend bool operator==(const Transaction&, const Transaction&);
	friend bool operator!=(const Transaction&, const Transaction&);
	friend bool operator>(const Transaction&, const Transaction&);
	friend bool operator<(const Transaction&, const Transaction&);
};

class CoinbaseTransaction : public Transaction {
public:
	/**	@brief	Default C'tor in order for Block C'tor to work */
	CoinbaseTransaction() {}
	/**	@brief	Construct a Coinbase Transaction (from port 0) given to port and amount */
	CoinbaseTransaction(unsigned short to, double amount) : Transaction(0, to, amount) {}
	/**	@brief	Construct a Coinbase Transaction (from port 0) given to port and random amount */
	CoinbaseTransaction(unsigned short to) : Transaction(0, to) {}
	/**	@brief	Construct a Coinbase Transaction given all the fields, necessary for jsonification */
	CoinbaseTransaction(unsigned int ID, string timestamp, unsigned short from, unsigned short to, double amount);
	/**	@brief	Construct a Coinbase Transaction from a Transaction from port 0 */
	CoinbaseTransaction(const Transaction&);
};



















