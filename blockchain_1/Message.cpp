#include "Message.h"

Message::Message(const Transaction& t)
{
	m_head			= Header::TRANSACTION;
	m_message.m_t	= t;
}

Message::Message(const Block& b)
{
	m_head = Header::BLOCK;
	m_message.m_b = b;
}

Message::Message(const vector<Block>& bb)
{
	m_head = Header::BLOCKCHAIN;
	m_message.m_bc = bb;
}

/**	@return	the string correspnding to the message header */
string Message::getHeaderString() const
{
	switch (m_head) {
	case Header::TRANSACTION:
		return(string("TRANSACTION"));
		break;
	case Header::BLOCK:
		return(string("BLOCK"));
		break;
	case Header::BLOCKCHAIN:
		return(string("BLOCKCHAIN"));
		break;
	default:
		return(string("NONE"));
	}
}

const Body& Message::decode() const
{
	Body res;
	switch (m_head) {
	case Header::TRANSACTION:
		res.m_t = this->m_message.m_t;
		break;
	case Header::BLOCK:
		res.m_b = this->m_message.m_b;
		break;
	case Header::BLOCKCHAIN:
		res.m_bc = this->m_message.m_bc;
		break;
	default:
		throw std::runtime_error("Message not a Transaction, Block or vector of Blocks");
	}
	return res;
}

const boost::variant< Transaction&, Block&, vector<Block>& > Message::decode_2() const
{
	switch (m_head) {
	case Header::TRANSACTION:
		return m_message.m_t;
		break;
	case Header::BLOCK:
		return m_message.m_b;
		break;
	case Header::BLOCKCHAIN:
		return m_message.m_bc;
		break;
	default:
		throw std::runtime_error("Message not a Transaction, Block or vector of Blocks");
	}
}

bool operator==(const Message& msg1, const Message& msg2)
{
	return	(msg1.m_head == msg2.m_head) &&
		(msg1.m_message == msg2.m_message);
}