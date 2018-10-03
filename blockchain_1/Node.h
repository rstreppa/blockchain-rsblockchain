/**
	Node.h
	Purpose:	creates a full node of a Blockchain Network, the core of the program

	@author rstreppa
	@version 1.0 07/11/2018
*/

#pragma once
#include <vector>
#include "Transaction.h"
#include "Block.h"
#include "BlockChain.h"
#include "Message.h"
#include "Config.h"
#include <boost/random.hpp>


using namespace std;


class Node {								///< there is only one Node, Singleton design pattern
private:
	double m_balance;						///< node account balance
	unsigned short m_port;					///< port where the server node will be running, immutable
	vector<unsigned short> m_portList;		///< vector of active ports, change over time
	BlockChain m_blockchain;				///< the unique active chain of the (unique) Node	

	vector<Transaction> m_unconfirmedPool;	///< pool of unconfirmed Transactions to be added to next mined Block 
	Block m_candidateblock;					///< candidate Block to be added to active chain
	vector<Block> m_candidatechain;			///< candidate BlockChain to repalce current active chain

	vector<Transaction> m_transactions;		///< vector of all Transactions since inception for bookkeeping
	vector<Block> m_blocks;					///< vector of all Blocks since inception for bookkeeping
	vector<Message> m_inMsgs;				///< vector of all incoming Messages since inception for bookkeeping
	vector<Message> m_outMsgs;				///< vector of all outgoing Messages since inception for bookkeeping
protected:
	/**	@brief	Construct a full node: set balance, server port and portlist, must succeed */
	Node(double balance = Constants::ACCOUNT) noexcept;
	/**	@brief	virtual destructor in case you want to derive from Node (hence protected) */
	virtual ~Node() = default;
	/**	@brief	forbid copy constructor: only one node */
	Node(const Node&) = delete;
	/**	@brief	forbid assignement operator: only one node */
	Node& operator=(const Node&) = delete;
public:
	/** @brief	create unique instance of a Node with optional balance argument */
	static Node& getInstance(double balance = Constants::ACCOUNT) noexcept(std::is_nothrow_constructible<Node>::value)
	{
		// Guaranteed to be destroyed.
		// Instantiated on first use.
		// Thread safe in C++11
		// Usage: Node& node = Node::getInstance(<optional balance>);
		static Node instance(balance);
		return instance;
	}
public:
	/**	@brief	retrieve current Node balance */
	double getBalance() const					{ return(m_balance); }
	/**	@brief	retrieve port permanently being used by the node as a server port  */
	unsigned short getPort() const				{ return(m_port); }
	/**	@brief	update list of active ports since nodes on (i.e. ports) change over times */
	void updatePortList();					
	/**	@brief	retrieve ports currently active i.e. occupied by nodes and able to get connections */
	vector<unsigned short> getPortList();	// not const since it updates the internal list of ports first
	/** @brief	make Genesis Block */
	Block makeGenesisBlock();
	/** @brief	update chain by adding a Block: either genesis or mined or arrived from another Node */
	bool updateChain(const Block& newblock);
	/** @brief	validate chain arrived from another Node by comparing with active one:
				1) must be a valid new chain
				2) must match the existing chain up to the last 6 Blocks, i.e. enough Proof-of-Work
				* if chain valid but exceeds 6 Block limit, send complement Transactions back to pool 
	*/
	bool validateChain(const vector<Block>& newchain);
	/** @brief	update chain arrived from another Node after validation */
	bool updateChain(const vector<Block>& newchain);
	/** @brief	send Block Transactions (+Coinbase) to pool, Send the set difference with active chain */
	void sendBackToPool(const Block& newblock);
	/** @brief	send Block Transactions (+Coinbase) to pool, Send the set difference with active chain */
	void sendBackToPool(const vector<Block>& newchain);
	/** @brief	remove Block Transactions (+Coinbase) from pool, remove the set difference */
	void removeFromPool(const Block& block);
	/** @brief	remove  BlockChain Transactions from pool, remove the set difference */
	void removeFromPool(const vector<Block>& chain);
	/** @brief	add a new Transaction (either generated or received as a Message) to pool */
	void addToPool(const Transaction& newtransaction);
	/** @brief	make a Transaction (separate thread):
				1) update balance
				2) add it to pool
				3) add it to the internal list of all Transactions
				4) package it in a Message and send it to the network
				5) add Message to internal list of Outgoing Messages for Bookkeeping
	*/
	void makeTransaction(boost::random::mt19937_64& rng);
	/** @brief	manage a Transaction received as a Message:
				1) update balance
				2) add it to pool
				3) add it to the internal list of all Transactions
	*/
	void manageTransaction();
	/** @brief	mine new Block (separate thread):
				1) solve computational puzzle as Proof-Of-Work
				2) create new valid Block, populate it with pool Transactions, empty pool 
				3) initialize internal candidate Block 
				4) store in bookkeping vector of Blocks
				5) package it in a Message and send it to the network (TOGETHER WITH ACTIVE CHAIN)
				6) add Messages (Block & BlockChain) to internal list of Outgoing Messages for Bookkeeping
	*/
	void mineBlock();
	/** @brief	manage a Block received as a Message or just mined:
			1) initialize internal candidate Block
			2) validate it
			3) if valid, add to active chain
			4) if invalid, send Transactions back to pool 
			5) add it to the internal bookkeping list of all Blocks
	*/
	void manageBlock();
	/** @brief	manage a BlockChain received as a Message:
			1) initialize internal candidate BlockChain
			2) validate it
			3) if valid update chain 
			4) if invalid, send complement Transactions back to pool
	*/
	void manageChain();
	/** @brief	manage an incoming Message:
			1) add Message to internal list of Incoming Messages for Bookkeeping
			2) check header and 
				2a) if Transaction manage Transaction
				2b) if Block manage Block
				2c) if BlockChain manage Chain
		@return success or failure
	*/
	bool manageIncomingMsg(const Message& newmessage);
};
