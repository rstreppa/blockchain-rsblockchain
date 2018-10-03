/**
Message.h
Purpose:	Class for serialized message transmission
			of Transactions, Blocks, BlockChain

@author rstreppa
@version 1.0 09/25/2018
*/

#pragma once
#include "Transaction.h"
#include "Block.h"
#include <vector>
#include <string>
#include "boost/variant.hpp"

using namespace std;

enum class Header : unsigned int { TRANSACTION, BLOCK, BLOCKCHAIN };	///< Header type of any message

struct Body					// Logically a Union, but cumbersome to code as such 
{
	Transaction m_t;		///< the body of a message can be either a Transaction
	Block m_b;				///< or a Block 
	vector<Block> m_bc;		///< or a vector of Blocks, i.e. a BlockChain

	/**	@brief	Default C'tor, necessary to compile Message C'tor*/
	Body() {}
	/**	@brief	Construct a Body given all the fields, necessary for jsonification */
	Body(Transaction t, Block b, vector<Block> bc) : m_t(t), m_b(b), m_bc(bc) {}
	/**	@brief	Copy Constructor */
	Body(const Body&) = default;
	/**	@brief	Assignement Operator */
	Body& operator=(const Body&) = default;
};

bool operator==(const Body& b1, const Body& b2)
{
	return	(b1.m_t == b2.m_t) &&
		(b1.m_b == b2.m_b) &&
		(b1.m_bc == b2.m_bc);
}

class Message
{
private:
	Header m_head;			///< Header of the Message
	Body m_message;			///< Message Body, i.e. a Transaction or a Block or a vector of Blocks
public:
	/**	@brief	Default C'tor */
	Message() {}
	/**	@brief	Construct a Message given Header and Body, necessary for jsonification */
	Message(Header head, Body message) : m_head(head), m_message(message) {}
	/**	@brief	Construct a Message from a Transaction */
	Message(const Transaction& t);
	/**	@brief	Construct a Message from a Block */
	Message(const Block& b);
	/**	@brief	Construct a Message from a vector of Blocks */
	Message(const vector<Block>& bb);
	/**	@brief	Copy Constructor */
	Message(const Message&) = default;
	/**	@brief	Assignement Operator */
	Message& operator=(const Message&) = default;
	
	/**	@brief	get the Message Header as a string */
	string getHeaderString() const;
	/**	@brief	get the Message Header as its type enum */
	const Header& getHeader() const	{ return m_head; }
	/**	@brief	get the Message Body */
	const Body& getMessage() const	{ return m_message; }
	/**	@brief	set the Message Header as its type enum */
	void setHeader(const Header& header) { this->m_head = header; }
	/**	@brief	set the Message Body (overloaded) */
	void setMessage(const Transaction& t)		{ this->m_message.m_t = t; }
	void setMessage(const Block& b)				{ this->m_message.m_b = b; }
	void setMessage(const vector<Block>& bc)	{ this->m_message.m_bc = bc; }

	/** @brief decode a Message into a Body */
	const Body& decode() const;

	/** @brief decode a Message into a Transaction, Block or vector of Blocks */
	const boost::variant< Transaction&, Block&, vector<Block>& > decode_2() const;

	/** @brief comparison operator */
	friend bool operator==(const Message&, const Message&);
};
