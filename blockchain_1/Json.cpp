#include "Json.h"
#include <memory>
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>


Object Json::obj(const Transaction& t)
{
	Object t_obj;
	t_obj.push_back(Pair("ID", t.getID()));
	t_obj.push_back(Pair("timestamp", t.getTimestamp()));
	t_obj.push_back(Pair("from", t.getFrom()));
	t_obj.push_back(Pair("to", t.getTo()));
	t_obj.push_back(Pair("amount", t.getAmount()));
	return(t_obj);
}

string Json::write(const Transaction& t)
{
	// return(write(obj(t), pretty_print, 2));
	return(write(Json::obj(t)));
}

vector<Object> Json::obj(const vector<Transaction>& tt)
{
	vector<Object> tt_obj;
	for (auto t : tt)
		tt_obj.push_back(Json::obj(t));
	return(tt_obj);
}

string Json::write(const vector<Transaction>& tt)
{
	vector<Object> tt_obj = Json::obj(tt);
	const Value val(tt_obj.begin(), tt_obj.end());
	// return(write(val, pretty_print, 2));
	return(write(val));
}

Transaction Json::read_t(const mValue& j_t)
{
	unsigned int ID = j_t.get_obj().at("ID").get_int();
	string timestamp = j_t.get_obj().at("timestamp").get_str();
	unsigned short from = (unsigned short)j_t.get_obj().at("from").get_int();
	unsigned short to = (unsigned short)j_t.get_obj().at("to").get_int();
	double amount = j_t.get_obj().at("amount").get_real();
	return Transaction(ID, timestamp, from, to, amount);
}

vector<Transaction> Json::read_tt(const mValue& j_tt)
{
	vector<Transaction> res;
	auto& j_tt_v = j_tt.get_array();
	for (unsigned int i = 0; i < j_tt_v.size(); i++)
		res.push_back(Json::read_t(j_tt_v[i].get_obj()));
	return res;
}

Object Json::obj(const BlockHeader& bh)
{
	Object bh_obj;
	bh_obj.push_back(Pair("index", (int)bh.index));
	bh_obj.push_back(Pair("prevHash", bh.prevHash));
	bh_obj.push_back(Pair("blockHash", bh.blockHash));
	bh_obj.push_back(Pair("merkleRoot", bh.merkleRoot));
	bh_obj.push_back(Pair("timestamp", bh.timestamp));
	bh_obj.push_back(Pair("difficulty", bh.difficulty));
	bh_obj.push_back(Pair("nonce", (int)bh.nonce));
	return(bh_obj);
}

string Json::write(const BlockHeader& bh)
{
	// return(write(obj(bh), pretty_print, 2));
	return(write(Json::obj(bh)));
}

BlockHeader Json::read_bh(const mValue& j_bh)
{
	unsigned int index	= j_bh.get_obj().at("index").get_int();
	string prevHash		= j_bh.get_obj().at("prevHash").get_str();
	string blockHash	= j_bh.get_obj().at("blockHash").get_str();
	string merkleRoot	= j_bh.get_obj().at("merkleRoot").get_str();
	string timestamp	= j_bh.get_obj().at("timestamp").get_str();
	double difficulty	= j_bh.get_obj().at("difficulty").get_real();
	unsigned int nonce	= j_bh.get_obj().at("nonce").get_int();
	return BlockHeader(index, prevHash, blockHash, merkleRoot, timestamp, difficulty, nonce);
}

Object Json::obj(const Block& b)
{
	Object b_obj;
	b_obj.push_back(Pair("index", (int)b.getIndex()));
	b_obj.push_back(Pair("prevHash", b.getPrevHash()));
	b_obj.push_back(Pair("blockHash", b.getBlockHash()));
	b_obj.push_back(Pair("merkleRoot", b.getMerkleRoot()));
	b_obj.push_back(Pair("timestamp", b.getTimestamp()));
	b_obj.push_back(Pair("difficulty", b.getDifficulty()));
	b_obj.push_back(Pair("nonce", (int)b.getNonce()));
	b_obj.push_back(Pair("blockheader", write(b.getBlockHeader())));
	b_obj.push_back(Pair("datum", write(b.getCoinbase())));
	b_obj.push_back(Pair("data", write(b.getTransactions())));
	return(b_obj);
}

vector<Object> Json::obj(const vector<Block>& bb)
{
	vector<Object> bb_obj;
	for (auto b : bb)
		bb_obj.push_back(Json::obj(b));
	return(bb_obj);
}

string Json::write(const Block& b)
{
	// return(write(Json::obj(b), pretty_print, 2));
	return(write(Json::obj(b)));
}

string Json::write(const vector<Block>& bb)
{
	vector<Object> bb_obj = Json::obj(bb);
	const Value val(bb_obj.begin(), bb_obj.end());
	// return(write(val, pretty_print, 2));
	return(write(val));
}

Block Json::read_b(const mValue& j_b)
{
	unsigned int index	= j_b.get_obj().at("index").get_int();
	string prevHash		= j_b.get_obj().at("prevHash").get_str();
	string blockHash	= j_b.get_obj().at("blockHash").get_str();
	string merkleRoot	= j_b.get_obj().at("merkleRoot").get_str();
	string timestamp	= j_b.get_obj().at("timestamp").get_str(); 
	double difficulty	= j_b.get_obj().at("difficulty").get_real();
	unsigned int nonce	= j_b.get_obj().at("nonce").get_int();
	
	string str_bh		= j_b.get_obj().at("blockheader").get_str();
	mValue j_bh;
	auto ok = read(str_bh, j_bh);
	if (!ok)
		throw runtime_error{ "invalid json" };
	BlockHeader bh		= Json::read_bh(j_bh.get_obj());

	string str_t		= j_b.get_obj().at("datum").get_str();
	mValue j_t;
	auto okk = read(str_t, j_t);
	if (!okk)
		throw runtime_error{ "invalid json" };
	CoinbaseTransaction t = CoinbaseTransaction(Json::read_t(j_t.get_obj()));

	string str_tt		= j_b.get_obj().at("data").get_str();
	mValue j_tt;
	auto okkk = read(str_tt, j_tt);
	if (!okkk)
		throw runtime_error{ "invalid json" };
	vector<Transaction> tt = Json::read_tt(j_tt);
	
	return Block(index, prevHash, blockHash, merkleRoot, timestamp, difficulty, nonce, bh, t, tt);
}

vector<Block> Json::read_bb(const mValue& j_bb)
{
	vector<Block> res;
	auto& j_bb_v = j_bb.get_array();
	for (unsigned int i = 0; i < j_bb_v.size(); i++)
		res.push_back(Json::read_b(j_bb_v[i].get_obj()));
	return res;
}

Object Json::obj(const Message& msg)
{
	Object m_obj;
	m_obj.push_back(Pair("header", msg.getHeaderString()));
	switch (msg.getHeader()) {
	case Header::TRANSACTION:
		m_obj.push_back(Pair("transaction", write(msg.getMessage().m_t)));
		break;
	case Header::BLOCK:
		m_obj.push_back(Pair("block", write(msg.getMessage().m_b)));
		break;
	case Header::BLOCKCHAIN:
		m_obj.push_back(Pair("blockchain", write(msg.getMessage().m_bc)));
		break;
	default:
		m_obj.push_back(Pair("none", ""));
	}
	return(m_obj);
}

string Json::write(const Message& msg)
{
	// return(write(obj(msg), pretty_print, 2));
	return(write(Json::obj(msg)));
}

Message Json::read_message(string& jdoc)
{
	Message msg;

	mValue m_parse;
	auto ok = read(jdoc, m_parse);
	if (!ok)
		throw runtime_error{ "invalid json" };

	auto header = m_parse.get_obj().at("header");

	if (header.get_str() == "TRANSACTION")
	{
		msg.setHeader(Header::TRANSACTION);
		string str_t = m_parse.get_obj().at("transaction").get_str();
		mValue j_t;
		auto ok = read(str_t, j_t);
		if (!ok)
			throw runtime_error{ "invalid json" };
		msg.setMessage(Json::read_t(j_t));
	}
	else if (header.get_str() == "BLOCK")
	{
		msg.setHeader(Header::BLOCK);
		string str_b = m_parse.get_obj().at("block").get_str();
		mValue j_b;
		auto ok = read(str_b, j_b);
		if (!ok)
			throw runtime_error{ "invalid json" };
		msg.setMessage(Json::read_b(j_b));
	}
	else if (header.get_str() == "BLOCKCHAIN")
	{
		msg.setHeader(Header::BLOCKCHAIN);
		string str_bc = m_parse.get_obj().at("blockchain").get_str();
		mValue j_bc;
		auto ok = read(str_bc, j_bc);
		if (!ok)
			throw runtime_error{ "invalid json" };
		msg.setMessage(Json::read_bb(j_bc));
	}
	else
		cerr << "error parsing Message json" << endl;

	return msg;
}

