#include "Transaction.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/random.hpp>
#include<boost/random/uniform_int_distribution.hpp>
#include "Config.h"
#include "Json.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;


Transaction::Transaction(unsigned short from, unsigned short to, double amount)
{
	m_ID++;
	ptime tt = boost::posix_time::microsec_clock::local_time();
	m_timestamp = to_simple_string(tt);
	m_from = from;
	m_to = to;
	m_amount = amount;
}
Transaction::Transaction(unsigned short from, unsigned short to, boost::random::mt19937_64& rng)
{
	m_ID++;
	ptime tt = boost::posix_time::microsec_clock::local_time();
	m_timestamp = to_simple_string(tt);
	m_from = from;
	m_to = to;
	size_t max = Constants::ACCOUNT;
	boost::random::uniform_int_distribution<> N(0, max);
	m_amount = double(N(rng) - max / 2) / 1000;
}
Transaction::Transaction(unsigned int ID, string timestamp, unsigned short from, unsigned short to, double amount) :
	m_ID(ID), m_timestamp(timestamp), m_from(from), m_to(to), m_amount(amount) 
{
}
Transaction& Transaction::operator=(const Transaction& other)
{
	if (this != &other)
	{
		m_ID = other.m_ID;
		m_timestamp = other.m_timestamp;
		m_from		= other.m_from;
		m_to		= other.m_to;
		m_amount = other.m_amount;
	}
	return *this;
}


string Transaction::toJson()
{
	return(Json::write(*this));
}


bool operator==(const Transaction& t1, const Transaction& t2)
{
	return	(t1.m_ID == t2.m_ID)				&&
			(t1.m_timestamp == t2.m_timestamp)	&&
			(t1.m_from == t2.m_from)			&&
			(t1.m_to == t2.m_to)				&&
			(t1.m_amount == t2.m_amount);
}

bool operator!=(const Transaction& t1, const Transaction& t2)
{
	return	!(t1 == t2);
}

bool operator>(const Transaction& t1, const Transaction& t2)
{
	ptime time1 = time_from_string(t1.getTimestamp());
	ptime time2 = time_from_string(t2.getTimestamp());
	return	time1 > time2;
}

bool operator<(const Transaction& t1, const Transaction& t2)
{
	ptime time1 = time_from_string(t1.getTimestamp());
	ptime time2 = time_from_string(t2.getTimestamp());
	return	time1 < time2;
}

CoinbaseTransaction::CoinbaseTransaction(unsigned int ID, string timestamp, unsigned short from, 
	unsigned short to, double amount) : Transaction(ID, timestamp, from, to, amount)
{
	if (from != 0)
		throw  runtime_error("Transaction not from port 0, not a Coinbase");
}

CoinbaseTransaction::CoinbaseTransaction(const Transaction& other) 
{
	if(other.getFrom() != 0)
		throw  runtime_error("Transaction not from port 0, cannot cast to Coinbase");
	CoinbaseTransaction(other.getID(), other.getTimestamp(), other.getFrom(), other.getTo(), other.getAmount());
}

