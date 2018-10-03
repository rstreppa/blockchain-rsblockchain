/**
BlockChain.h
Purpose:	BlockChain class for nodes: implemented as vector of unique_ptr

@author rstreppa
@version 1.0 09/14/2018
*/

#pragma once
#include <vector>
#include <memory>
#include "Block.h"
#include <string>
#include "Json.h"

using namespace std;

class BlockChain {
private:
	vector< unique_ptr<Block> > m_blockchain;	///< vector of unique pointers to block is the unique blockchain  
public:
	/** @brief	default C'tor initialize empty vector of unique pointers to Block */
	BlockChain() {}
	/**	@brief	Construct a BlockChain by setting all blocks, necessary for jsonification and for replacing chain */
	BlockChain(const vector<Block>& replacement);
	
	/** @brief	get Block by position in the chain */
	const Block& getBlock(const int index);
	/** @brief	get Block by its hash */
	const Block& getBlock(const string hash);
	/** @brief	get number of Blocks in the chain */
	size_t size();
	/** @brief	convert Blockchain to a vector of Blocks */
	vector<Block> getRawChain();	// note that return by value, you don't want to mess with the internals
	/** @brief	get hash of the most recent block (to validate the next) */
	string getLatestBlockHash();
	/** @brief	get most recent block */
	const Block& getLatestBlock();
	/** @brief	get index of most recent Block */
	unsigned int getLatestBlockIndex();
	/** @brief	validate Block: 
				1) valid block 
				2) re-evaluate merkle 
				3) greater timestamp 
				4) match previous hash 
	*/
	bool validateBlock(const Block& newblock);
	/** @brief	add block to the current chain after validation */
	bool addBlock(const Block& newblock);
	/** @brief	valid BlockChain if: 
				1) each Block is valid 
				2) timestamps in sequence 
				3) Block's prevHash is the the hash of the previous Block
	*/
	bool isValid();
	/** @brief	replace BlockChain with a vector of Blocks: LEAVE CONTROL TO INVOKE IT TO Node CLASS */
	bool replaceChain(const vector<Block>& replacement);
	/** @brief	turn Blockchain into Json string */
	string toJSON();

	/** @brief comparison operators (BlockChain unique, so no operators as for Block or Transaction) */
	bool eq(const vector<Block>& other);
	bool neq(const vector<Block>& other);
	bool gt(const vector<Block>& other);
	bool lt(const vector<Block>& other);
};
