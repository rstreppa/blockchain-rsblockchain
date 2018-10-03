#include "Block.h"
#include "Utility.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Config.h"
#include "Merkle.h"
#include "Json.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

constexpr double COINBASE_AMOUNT = Constants::ACCOUNT / 1000;

BlockHeader::BlockHeader(unsigned int index_, string prevHash_, string blockHash_,
	string merkleRoot_, string timestamp_, double difficulty_, unsigned int nonce_) :
	index(index_), prevHash(prevHash_), blockHash(blockHash_), merkleRoot(merkleRoot_),
	timestamp(timestamp_), difficulty(difficulty_), nonce(nonce_)
{}

bool operator==(const BlockHeader& bh1, const BlockHeader& bh2)
{
	return	(bh1.index == bh2.index) &&
			(bh1.prevHash == bh2.prevHash) &&
			(bh1.blockHash == bh2.blockHash) &&
			(bh1.merkleRoot == bh2.merkleRoot) &&
			(bh1.timestamp == bh2.timestamp) &&
			(bh1.difficulty == bh2.difficulty) &&
			(bh1.nonce == bh2.nonce);
}


Block::Block(unsigned int index, string prevHash, string blockHash, string merkleRoot,
	string timestamp, double difficulty, unsigned int nonce, BlockHeader bh,
	CoinbaseTransaction datum, vector<Transaction> data) :
	m_index(index), m_prevHash(prevHash), m_blockHash(blockHash), m_merkleRoot(merkleRoot),
	m_timestamp(timestamp), m_difficulty(difficulty), m_nonce(nonce), m_bh(bh), m_datum(datum), m_data(data)
	{
	}

Block::Block(unsigned int index, string prevHash, string blockHash, string merkleRoot, string timestamp,
	double difficulty, unsigned int nonce, CoinbaseTransaction datum, vector<Transaction> data) : 
	m_index(index), m_prevHash(prevHash), m_blockHash(blockHash), m_merkleRoot(merkleRoot),
	m_timestamp(timestamp), m_difficulty(difficulty), m_nonce(nonce), m_datum(datum), m_data(data)
{
	m_bh = BlockHeader::BlockHeader(index, prevHash, blockHash, merkleRoot, timestamp, difficulty, nonce);
}


/**
	@return		the fixed-length SHA-512 hash of the Block Header
*/
string Block::hash()
{
	string res = to_string(m_index) + m_prevHash + m_merkleRoot + m_timestamp + to_string(m_difficulty) + to_string(m_nonce);
	return sha512(res);
}

/**
	@return		the fixed-length SHA-512 hash of the Block Header
*/
string Block::computeMerkleRoot()	const
{
	vector<Transaction> res = m_data;
	Transaction  additional	= m_datum;	// upcast, dynamic_cast may be used, but unnecessary
	res.push_back(additional);
	Merkle<string> merkle(hashThem);
	for (auto t : res)
		merkle.add(Json::write(t));
	return merkle.root();
}

void Block::updateMerkleRoot()
{
	m_merkleRoot = this->computeMerkleRoot();
}

void Block::updateHash()
{
	m_blockHash = this->hash();
}

/**
	@return		whether Block is valid or not
*/
bool Block::isValid() const
{
	Block copy = *this;
	return(m_blockHash == copy.hash());
}


/**
	@return		the Jsonified string of the Block
*/
string Block::toJson()
{
	return(Json::write(*this));
}


Block::Block(string prevHash, double difficulty, unsigned int nonce, unsigned short toPort, 
	vector<Transaction> data)
{
	m_index++;			
	m_prevHash		= prevHash;
	m_datum			= CoinbaseTransaction(toPort, COINBASE_AMOUNT);
	m_data			= data;
	m_merkleRoot	= computeMerkleRoot();
	ptime tt		= boost::posix_time::microsec_clock::local_time();
	m_timestamp		= to_simple_string(tt);
	m_difficulty	= difficulty;
	m_nonce			= nonce;
	m_blockHash		= hash();
	m_bh			= BlockHeader(m_index, m_prevHash, m_blockHash, m_merkleRoot, 
		m_timestamp, m_difficulty, m_nonce);
}

bool operator ==(const Block& b1, const Block& b2)
{
	return	(b1.m_index == b2.m_index) &&
		(b1.m_prevHash == b2.m_prevHash) &&
		(b1.m_blockHash == b2.m_blockHash) &&
		(b1.m_merkleRoot == b2.m_merkleRoot) &&
		(b1.m_timestamp == b2.m_timestamp) &&
		(b1.m_difficulty == b2.m_difficulty) &&
		(b1.m_nonce == b2.m_nonce) &&
		(b1.m_bh == b2.m_bh) &&
		(b1.m_datum == b2.m_datum) &&
		(b1.m_data == b2.m_data);
}

bool operator!=(const Block& b1, const Block& b2)
{
	return	!(b1 == b2);
}

bool operator>(const Block& b1, const Block& b2)
{
	ptime time1 = time_from_string(b1.getTimestamp());
	ptime time2 = time_from_string(b2.getTimestamp());
	return	time1 > time2;
}

bool operator<(const Block& b1, const Block& b2)
{
	ptime time1 = time_from_string(b1.getTimestamp());
	ptime time2 = time_from_string(b2.getTimestamp());
	return	time1 < time2;
}

