/**
Json.h
Purpose:	namespace Json to jsonify messages using json_spirit 

@author rstreppa
@version 1.0 09/13/2018
*/

#pragma once
#include "json_spirit.h"
#include <vector>
#include <string>
#include "Transaction.h"
#include "Block.h"
#include "Message.h"

#ifndef JSON_SPIRIT_VALUE_ENABLED
#error Please define JSON_SPIRIT_VALUE_ENABLED for the Value type to be enabled 
#endif

using namespace std;
using namespace json_spirit;

namespace Json {
	/**
		@brief		turn Transaction into json_spirit Object
		@param[t]	Transaction object

		@return		json_spirit Object
	*/
	Object obj(const Transaction& t);

	/**
		@brief		write Transaction into string via json_spirit
		@param[t]	Transaction object

		@return		jsonified (string) Transaction
	*/
	string write(const Transaction& t);

	/**
		@brief		turn Transaction vector into json_spirit Object vector
		@param[tt]	Transaction vector

		@return		json_spirit Object vector
	*/
	vector<Object> obj(const vector<Transaction>& tt);

	/**
		@brief		write Transaction vector into string via json_spirit
		@param[tt]	Transaction vector

		@return		jsonified (string) Transaction vector
	*/
	string write(const vector<Transaction>& tt);

	/**
		@brief		read json object into Transaction
		@param[j_t]	json object (transaction)

		@return		Transaction object
	*/
	Transaction read_t(const mValue& j_t);

	/**
		@brief			read json object into Transaction vector
		@param[j_tt]	json object (transaction vector)

		@return			Transaction vector object
	*/
	vector<Transaction> read_tt(const mValue& j_tt);

	/**
		@brief		turn BlockHeader into json_spirit Object
		@param[bh]	BlockHeader object

		@return		json_spirit Object
	*/
	Object obj(const BlockHeader& bh);

	/**
		@brief		write BlockHeader into string via json_spirit
		@param[bh]	BlockHeader object

		@return		jsonified (string) BlockHeader
	*/
	string write(const BlockHeader& bh);

	/**
		@brief			read json object into BlockHeader
		@param[j_bh]	json object (BlockHeader)

		@return			BlockHeader object
	*/
	BlockHeader read_bh(const mValue& j_bh);

	/**
		@brief		turn Block into json_spirit Object
		@param[b]	Block object

		@return		json_spirit Object
	*/
	Object obj(const Block& b);

	/**
		@brief		turn Block vector into json_spirit Object vector
		@param[tt]	Block vector

		@return		json_spirit Object vector
	*/
	vector<Object> obj(const vector<Block>& bb);

	/**
		@brief		write Block into string via json_spirit
		@param[b]	Block object

		@return		jsonified (string) Block
	*/
	string write(const Block& b);

	/**
		@brief		write Block vector into string via json_spirit
		@param[bb]	Block vector

		@return		jsonified (string) Block vector
	*/
	string write(const vector<Block>& bb);

	/**
		@brief		read json object into Block
		@param[j_b]	json object (Block)

		@return		Block object
	*/
	Block read_b(const mValue& j_b);

	/**
		@brief			read json object into Block vector
		@param[j_bb]	json object (block vector)

		@return			Block vector object
	*/
	vector<Block> read_bb(const mValue& j_bb);

	/**
		@brief		turn Message into json_spirit Object
		@param[msg]	Message object

		@return		json_spirit Object
	*/
	Object obj(const Message& msg);

	/**
		@brief		write Message into string via json_spirit
		@param[msg]	Message object

		@return		jsonified (string) Message
	*/
	string write(const Message& msg);

	/**
		@brief			read json object into Message
		@param[jdoc]	json object (Message)

		@return			Message object
	*/
	Message read_message(string& jdoc);
};