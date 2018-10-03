/**
Block.h
Purpose:	Block class for nodes

@author rstreppa
@version 1.0 09/14/2018
*/
#pragma once

#include <vector>
#include <string>
#include "Transaction.h"

using namespace std;

struct BlockHeader {
	unsigned int index;		///< sequential block identifier  
	std::string prevHash;	///< previous block hash
	std::string blockHash;	///< block hash
	std::string merkleRoot;	///< merkle root (hash of the tree of transactions)
	std::string timestamp;	///< block timestamp
	double difficulty;		///< target mining difficulty
	unsigned int nonce;		///< random number (mining "computational puzzle")

	/**	@brief	Defualt C'tor, necessary to compile Block C'tor*/
	BlockHeader() {}
	/**	@brief	Construct a BlockHeader given all the fields, necessary for jsonification */
	BlockHeader(unsigned int index_, std::string prevHash_, std::string blockHash_, 
		std::string merkleRoot_, std::string timestamp_, double difficulty_, unsigned int nonce_);
	/**	@brief	Copy Constructor */
	BlockHeader(const BlockHeader&) = default;
	/**	@brief	Assignement Operator */
	BlockHeader& operator=(const BlockHeader&) = default;
	/** @brief comparison operator */
	friend bool operator==(const BlockHeader&, const BlockHeader&);
};


class Block
{
private:
	unsigned int m_index;			///< sequential block identifier  
	string m_prevHash;				///< previous block hash
	string m_blockHash;				///< block hash
	string m_merkleRoot;			///< merkle root (hash of the tree of transactions)
	string m_timestamp;				///< block timestamp
	double m_difficulty;			///< target mining difficulty
	unsigned int m_nonce;			///< random number (mining "computational puzzle")
	BlockHeader m_bh;				///< Block Header
	CoinbaseTransaction m_datum;	///< Block coinbase transaction (from port 0, i.e. nowhere)
	vector<Transaction> m_data;		///< Ordinary Transactions
public:
	/**	@brief	Default C'tor, necessary to compile Message C'tor*/
	Block() {}
	/**	@brief	Construct a Block given all the fields, necessary for jsonification */
	Block(unsigned int index, string prevHash, string blockHash, string merkleRoot,
		string timestamp, double difficulty, unsigned int nonce, BlockHeader bh,
		CoinbaseTransaction datum, vector<Transaction> data);
	/**	@brief	Construct a Block given all the fields, except BlockHeader (deduced) */
	Block(unsigned int index, string prevHash, string blockHash, string merkleRoot,
		string timestamp, double difficulty, unsigned int nonce,
		CoinbaseTransaction datum, vector<Transaction> data);
	/**	@brief	Construct a Block initializing whatever can be initialized */
	Block(string prevHash, double difficulty, unsigned int nonce, unsigned short toPort, 
		vector<Transaction> data);
	/**	@brief	Copy Constructor */
	Block(const Block&) = default;
	/**	@brief	Assignement Operator */
	Block& operator=(const Block&) = default;

	/**	@brief	Hash the Block Header */
	string hash();
	/**	@brief	compute the hashed Block Transactions */
	string computeMerkleRoot()	const;
	/** @brief update hashed Block Transactions */
	void updateMerkleRoot();
	/** @brief update Block hash */
	void updateHash();
	/** @brief check if a block is valid (match block hash) */
	bool isValid()	const;
	/** @brief convert Block to Json */
	string toJson();

	/**	@brief access Block sequential Identitifer (not unique) */
	unsigned int getIndex()							const	{ return m_index; }
	/**	@brief access Block previous hash */
	string getPrevHash()							const	{ return m_prevHash; }
	/**	@brief access Block hash */
	string getBlockHash()							const	{ return m_blockHash; }
	/**	@brief access merkle root (hash of the tree of transactions) */
	string getMerkleRoot()							const	{ return m_merkleRoot; }
	/**	@brief access block timestamp */
	string getTimestamp()							const	{ return m_timestamp; }
	/**	@brief access target mining difficulty */
	double getDifficulty()							const	{ return m_difficulty; }
	/**	@brief access nonce (mining random number for computational puzzle) */
	unsigned int getNonce()							const	{ return m_nonce; }
	// the next getter methods return reference to const to non simple types
	// 1) avoid overhead you would have returning by value and return the real internal object instead
	// 2) but return a const so you are not allowed to change the private parts
	/**	@brief access Block Header */
	const BlockHeader& getBlockHeader()				const	{ return m_bh; }
	/**	@brief access CoinbaseTransaction */
	const CoinbaseTransaction& getCoinbase()		const	{ return m_datum; }
	/**	@brief access Ordinary Transactions */
	const vector<Transaction>& getTransactions()	const	{ return m_data; }
	/** @brief comparison operators */
	friend bool operator==(const Block&, const Block&);
	friend bool operator!=(const Block&, const Block&);
	friend bool operator>(const Block&, const Block&);
	friend bool operator<(const Block&, const Block&);
};