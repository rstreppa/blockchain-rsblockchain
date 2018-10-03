/**
Utility.h
Purpose:	Utility functions
			maybe to be put in a class

@author rstreppa
@version 1.0 07/12/2018
*/

#pragma once
#include <vector>
#include <string>
#include <boost/random.hpp>
#include <chrono>


using namespace std;
using namespace std::chrono;



/**
	@brief			Returns whether the node accepts connections on a given port

	@param[port]	port the port being tested 
	@return			connection  accepted / not accepted on the port being tested
*/
bool accepting_connections(unsigned short port);

/**
	@brief			Returns always the first available port to set up a server
					among those given in input, i.e. the first port where a p2p connection CANNOT 
					be established because the prot is not taken yet

	@param[ports]	vector of ports available to set up a server 
	@return			the port to establish a server
*/
unsigned short serverPort(const vector<unsigned short> ports);

/**
	@brief		Returns the SHA-512 hash (unbroken) generated from an arbitrarily long string

	@param[str]	str arbitrarily long input string
	@return		the fixed-length SHA-512 hash
*/
string sha512(const string& str);

/**
	@brief			Returns the SHA-512 hash (unbroken) generated from an arbitrarily large file

	@param[file]	filename (including path)
	@return			the fixed-length SHA-512 hash
*/
string sha512(const char* file);

/**
	@brief		Returns the SHA-512 hash (unbroken) generated from two concatenated strings

	@param[a]	first input string
	@param[b]	second input string
	@return		the fixed-length SHA-512 hash
*/
string hashThem(const string& a, const string& b);

/**
	@brief			output a random number between 0 and a max sampled from a Uniform Distribution

	@param[max]		maximum integer permitted
	@param[seed]	seed of the random generator (usually linked to the server port)
	@return			unsigned integer between 0 and max			
*/
size_t uRandom(const size_t max, const unsigned short seed);

/**
	@brief			output a random number between 0 and a max sampled from a Uniform Distribution

	@param[max]		maximum integer permitted
	@param[rng]		random number generator whose state is modified 
	@return			unsigned integer between 0 and max
*/
size_t uRandom(const size_t max, boost::random::mt19937_64& rng);

/**
	@brief	struct containing return values of the computational puzzle			
*/
struct ProofOfWork {
	double difficulty;			///< number of nonces tried divided by POWFACTOR 
	unsigned int nonce;			///< nonce solving the SHA512 compuational puzzle
	double seconds;				///< time in seconds necessary to solve the SHA512 puzzle
};

/**
	@brief			computational puzzle solver (with current settings in Config.h takes up to 2 minutes)

	@param[rng]		reference to random number generator so as not to reset the state
	@return			POW structure: difficulty, solving nonce and elapsed time  
*/
ProofOfWork powSolver(boost::random::mt19937_64& rng)

