/**
	Config.h
	Purpose:	Config file for the whole program: constants and such 

	@author rstreppa
	@version 1.0 07/11/2018
*/

#pragma once
#include <string>
#include <vector>

namespace Constants {
	const std::string localhost("127.0.0.1");						///< IP address of the localhost
	/** all ports of the localhost used by the intercommunicating nodes */
	const std::vector<unsigned short> ports = { 8080, 8081, 8082, 8083, 8084, 8085, 8086, 8087, 8088, 8089,
		8090, 8091, 8092, 8093, 8094, 8095, 8096, 8097, 8098, 8099 };
	const size_t nPorts = ports.size();								///< number of available ports
	const size_t BUFLEN = 16384;									///< length of a buffer for hash function sha512	
	const size_t MSGLEN = 10000000;									///< allocate enough chars for a long message like a long BlockChain
	const size_t ACCOUNT = 1000000;									///< size of an initial account of a node
	const size_t LONGEST_CHAIN_MISMATCH = 6;						///< max number of block difference new chain is allowed
																	///< to have to replace the old one
	const size_t SLEEP_TRANSACTION = 10;							///< max time in seconds sleep before a transaction is made
	const size_t MAXNONCE = 1000000000;								///< maximum nonce allowed	
	const unsigned long long TARGET = 1000000000000000;				///< target for the POW computational puzzle
	const double POWFACTOR = (double)10000000;						///< factor to compute POW Difficulty 				
};