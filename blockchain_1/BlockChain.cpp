#include "BlockChain.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;


BlockChain::BlockChain(const vector<Block>& replacement)
{
	m_blockchain.clear();
	for (auto block : replacement)
		m_blockchain.push_back(make_unique<Block>(block));
}

const Block& BlockChain::getBlock(const int index)
{
	if (index == -1)
		return *m_blockchain.back();
	else
		return *m_blockchain.at(index);
}


const Block& BlockChain::getBlock(const string hash)
{
	for (size_t i = 0; i < m_blockchain.size(); i++)
	{
		auto block = *m_blockchain[i];
		if (hash == block.getBlockHash())
			return block;
	}
	throw runtime_error("Error in getBlock, cannot find hash");
}

size_t BlockChain::size()
{
	return m_blockchain.size();
}

vector<Block> BlockChain::getRawChain()
{
	vector<Block> res;
	for (size_t i = 0; i < m_blockchain.size(); i++)
		res.push_back(*m_blockchain[i]);
	return res;
}

string BlockChain::getLatestBlockHash()
{
	return getBlock(-1).getBlockHash();
}

const Block& BlockChain::getLatestBlock()
{
	return getBlock(-1);
}

unsigned int BlockChain::getLatestBlockIndex()
{
	return m_blockchain.size();
}

bool BlockChain::validateBlock(const Block& newblock)
{
	// 1) valid block
	bool b1 = newblock.isValid();
	// 2) re - evalute merkle
	bool b2 = newblock.getMerkleRoot() == newblock.computeMerkleRoot() ? true : false;
	// 3) greater timestamp
	ptime last_t = time_from_string(this->getLatestBlock().getTimestamp());
	ptime new_t  = time_from_string(newblock.getTimestamp());
	bool b3 = new_t > last_t ? true : false;
	// 4) match previous hash
	bool b4 = this->getLatestBlock().getBlockHash() == newblock.getPrevHash() ? true : false;
	
	return b1 && b2 && b3 && b4;
}

bool BlockChain::addBlock(const Block& newblock)
{
	if (!this->validateBlock(newblock))
		return false;
	m_blockchain.push_back(make_unique<Block>(newblock));
	return true;
}

bool BlockChain::isValid()
{
	vector<ptime> timestamps;
	vector<string> prevHashes;
	vector<string> blockHashes;

	for (size_t i; i < this->size(); i++)
	{
		// 1) each Block is valid
		if (!m_blockchain[i]->isValid())
			return false;
		timestamps.push_back(time_from_string(m_blockchain[i]->getTimestamp()));
		prevHashes.push_back(m_blockchain[i]->getPrevHash());
		blockHashes.push_back(m_blockchain[i]->getBlockHash());
	}
	
	// 2) timestamps in sequence
	if(!is_sorted(timestamps.begin(), timestamps.end()))
		return false;
	
	// 3) Block's prevHash is the the hash of the previous Block
	vector<string>::const_iterator bf  = blockHashes.begin();
	vector<string>::const_iterator bl  = blockHashes.end() - 1;
	vector<string>::const_iterator pf  = prevHashes.begin() + 1;
	vector<string>::const_iterator pl  = prevHashes.end();
	vector<string> lhs(bf, bl);
	vector<string> rhs(pf, pl);
	if (!(lhs == rhs))
		return false;

	return true;
}

bool BlockChain::replaceChain(const vector<Block>& replacement)
{
	try
	{
		m_blockchain.clear();
		for (auto block : replacement)
			m_blockchain.push_back(make_unique<Block>(block));
		return true;
	}
	catch(const exception& e)
	{
		cout << e.what();
		return false;
	}
}

string BlockChain::toJSON()
{
	vector<Block> raw = this->getRawChain();
	return(Json::write(raw));
}

bool BlockChain::eq(const vector<Block>& other)
{
	return this->getRawChain() == other;
}

bool BlockChain::neq(const vector<Block>& other)
{
	return !this->eq(other);
}

bool BlockChain::gt(const vector<Block>& other)
{
	return this->size() > other.size();
}

bool BlockChain::lt(const vector<Block>& other)
{
	return this->size() < other.size();
}



