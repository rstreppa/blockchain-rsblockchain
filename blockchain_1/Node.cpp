#include "Node.h"
#include "Config.h"
#include "Utility.h"
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <algorithm>
#include <iterator>
#include<boost/random/uniform_int_distribution.hpp>
#include "Peer2Peer.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

Node::Node(double balance) noexcept
{
	m_balance = balance;					// set money balance to the default
	m_port = serverPort(Constants::ports);	// test available ports and choose next available to run the server
							  				// port stays the same through lifetime of the Node i.e. program	
	for (auto port : Constants::ports)
		if (accepting_connections(port) && (port != m_port))
			m_portList.push_back(port);
}

void Node::updatePortList()
{
	m_portList.clear();
	for (auto port : Constants::ports)
		if (accepting_connections(port) && (port != m_port))
			m_portList.push_back(port);
}

vector<unsigned short> Node::getPortList()
{
	this->updatePortList();
	return m_portList;
}

Block Node::makeGenesisBlock()
{
	unsigned int index		= 0;
	std::string prevHash	= "";
	std::string blockHash	= "";
	std::string merkleRoot	= "";
	ptime tt				= boost::posix_time::microsec_clock::local_time();
	std::string timestamp	= to_simple_string(tt);
	double difficulty		= 0;
	unsigned int nonce	= 0;
	CoinbaseTransaction datum;
	vector<Transaction> data;
	Block res  = Block(index, prevHash, blockHash, merkleRoot, timestamp, difficulty, nonce, datum, data);
	res.updateMerkleRoot();
	res.updateHash();
	return res;
}

bool Node::updateChain(const Block& newblock)
{
	// case 1) only a Genesis Block can be added to an empty BlockChain  
	if (m_blockchain.size() == 0)
	{
		Block genesis = this->makeGenesisBlock();
		if (newblock.getIndex()			== genesis.getIndex() && 
			newblock.getPrevHash()		== genesis.getPrevHash() && 
			newblock.getMerkleRoot()	== genesis.getMerkleRoot() &&
			newblock.getDifficulty()	== genesis.getDifficulty() &&
			newblock.getNonce()			== genesis.getNonce())
		{
			m_blockchain.addBlock(newblock);
			return true;
		}
		else
		{
			return false;
		}
	}
	
	// case 2) a newly mined Block or case 3) a Block received will be added to a non-empty BlockChain if valid
	if (m_blockchain.addBlock(newblock))			// addBlock performs the whole Block validation in the chain 
	{
		removeFromPool(newblock);					// remove Transactions from pool
		return true;
	}
	else
	{
		sendBackToPool(newblock);					// send Block Transactions back to pool
													// TODO: improve logic. What if fake Block Transactions?
		return false;								
	}
}

void Node::sendBackToPool(const Block& newblock)
{
	// get Transactions from the new Block
	vector<Transaction> tblock = newblock.getTransactions();
	tblock.push_back(newblock.getCoinbase());

	// get Transactions from from the active chain
	vector<Transaction> tchain;
	vector<Block> activechain = m_blockchain.getRawChain();
	for (auto block : activechain)
	{
		for (auto transaction : block.getTransactions())
			tchain.push_back(transaction);
		tchain.push_back(block.getCoinbase());
	}

	// find the difference
	std::vector<Transaction> diff;
	std::set_difference(tblock.begin(), tblock.end(), tchain.begin(), tchain.end(),
		std::inserter(diff, diff.begin()));
	
	// send to pool
	for (auto t : diff)
		m_unconfirmedPool.push_back(t);
}

void Node::sendBackToPool(const vector<Block>& newchain)
{
	// get Transactions from from the new chain
	vector<Transaction> tnewchain;
	for (auto block : newchain)
	{
		for (auto t : block.getTransactions())
			tnewchain.push_back(t);
		tnewchain.push_back(block.getCoinbase());
	}
	
	// get Transactions from from the active chain
	vector<Transaction> tchain;
	vector<Block> activechain = m_blockchain.getRawChain();
	for (auto block : activechain)
	{
		for (auto transaction : block.getTransactions())
			tchain.push_back(transaction);
		tchain.push_back(block.getCoinbase());
	}

	// find the difference
	std::vector<Transaction> diff;
	std::set_difference(tnewchain.begin(), tnewchain.end(), tchain.begin(), tchain.end(),
		std::inserter(diff, diff.begin()));

	// send to pool
	for (auto t : diff)
		m_unconfirmedPool.push_back(t);
}

void Node::removeFromPool(const Block& block)
{
	// get Transactions from the Block
	vector<Transaction> tblock = block.getTransactions();
	tblock.push_back(block.getCoinbase());

	// find the difference
	std::vector<Transaction> diff;
	std::set_difference(m_unconfirmedPool.begin(), m_unconfirmedPool.end(), tblock.begin(), tblock.end(),
		std::inserter(diff, diff.begin()));

	// set the pool to the difference
	m_unconfirmedPool.clear();
	m_unconfirmedPool = diff;
}

void Node::removeFromPool(const vector<Block>& chain) 
{
	// get Transactions from from the chain
	vector<Transaction> tchain;
	for (auto block : chain)
	{
		for (auto t : block.getTransactions())
			tchain.push_back(t);
		tchain.push_back(block.getCoinbase());
	}

	// find the difference
	std::vector<Transaction> diff;
	std::set_difference(m_unconfirmedPool.begin(), m_unconfirmedPool.end(), tchain.begin(), tchain.end(),
		std::inserter(diff, diff.begin()));

	// set the pool to the difference
	m_unconfirmedPool.clear();
	m_unconfirmedPool = diff;
}

void Node::addToPool(const Transaction& newtransaction)
{
	m_unconfirmedPool.push_back(newtransaction);
}

bool Node::validateChain(const vector<Block>& newchain) 
{
	// BlockChain is a vector of unique pointers, so you need some wrangling  
	vector<Block> oldchain = m_blockchain.getRawChain();		// get old chain
	BlockChain(newchain);										// replace with new chain 
	
	// new chain must be a valid new chain
	if (!m_blockchain.isValid())
	{
		BlockChain(oldchain);									// switch back
		return false;
	}

	// new chain must be strictly longer than old chain
	if (m_blockchain.size() <= oldchain.size())
	{
		BlockChain(oldchain);									// switch back
		return false;
	}

	// chains must mismatch at most up to the limit in Config.h (currently 6 Blocks)
	auto oldb = oldchain.begin();
	auto olde = oldchain.end();
	auto newb = newchain.begin();
	auto newe = newchain.end();
	while (oldb != olde && *oldb == *newb)			 
		++oldb, ++newb;										// equivalent to std::mismatch
	if (newe - newb > Constants::LONGEST_CHAIN_MISMATCH)
	{
		BlockChain(oldchain);									// switch back
		return false;
	}

	// all tests passed: switch back, a little clumsy but return true
	BlockChain(oldchain);									// switch back
	return true;
}

bool Node::updateChain(const vector<Block>& newchain)
{
	if (validateChain(newchain)) 
	{
		BlockChain(newchain);						// now newchain is a BlockChain not a vector of Blocks
		removeFromPool(newchain.getRawChain());		// remove set difference Transactions from pool
		return true;
	}
	sendBackToPool(newchain);						// send chain Transaction to pool
													// TODO: improve logic. What if fake chain Transactions? 
	return false;
}

void  Node::makeTransaction(boost::random::mt19937_64& rng)
{
	// select ports: from self to random running Node
	unsigned short from = m_port;
	updatePortList();
	boost::random::uniform_int_distribution<> N(0, m_portList.size()-1);
	unsigned short to = m_portList.at(N(rng));

	
	Transaction t(from, to, rng);		// generate Transaction 
	m_balance -= t.getAmount();			// update balance
	m_unconfirmedPool.push_back(t);		// add it to pool
	m_transactions.push_back(t);		// add it to the internal list of all Transactions
	
	// package Transaction in a Message and send it to the network
	Message msg(t);
	Peer2Peer p2p(*this);
	p2p.broadcast(msg);
	// add Message to internal list of Outgoing Messages for Bookkeeping
	m_outMsgs.push_back(msg);

}