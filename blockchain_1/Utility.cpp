#include "Utility.h"
#include <boost\asio.hpp>
#include <vector>
#include <string>
#include <fstream>   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "sha2.h"
#include "Config.h"

#include <boost/random.hpp>
#include<boost/random/uniform_int_distribution.hpp>


using namespace std;

	
bool accepting_connections(unsigned short port)
{
	using namespace boost::asio;
	using boost::asio::ip::tcp;
	using ec = boost::system::error_code;

	bool result = false;

	try
	{
		io_service svc;
		tcp::socket s(svc);
		deadline_timer tim(svc, boost::posix_time::seconds(1));

		tim.async_wait([&](ec) { s.cancel(); });
		s.async_connect({ {}, port }, [&](ec ec) {
			result = !ec;
		});

		svc.run();
	}
	catch (...) {}

	return result;
}

unsigned short serverPort(const vector<unsigned short> ports) 
{
	for (auto port : ports)
		if (!accepting_connections(port)) 
			return(port);
}

string sha512(const string& str)
{
	// Return by value. If you return a reference from a function, you must ensure that the object 
	// to which it refers will still exist after the function returns. 
	// Since objects with automatic storage duration are destroyed at the end of the block 
	// in which they are declared, they are guaranteed not to exist after the function returns.
	
	// VERY ugly solution, but there is no equivalent in Windows of fmemopen, the function that 
	// creates a file in memory; all implementations of fmemopen I found don't work
	ofstream file("tempfile.txt");
	if (file.is_open())
	{
		file << str;
		file.close();
	}
	return(sha512("tempfile.txt"));
}

string sha512(const char* file)
{
	FILE* in = nullptr;
	if ((in = fopen(file, "r")) == NULL) 
	{
		perror("Cannot read file in sha512(const char* file)");
		exit(-1);
	}
	int fd = fileno(in);

	SHA512_CTX	ctx512;
	SHA512_Init(&ctx512);
	unsigned char buf[Constants::BUFLEN];	
	int kl = 0;
	int l = 0;
	while((l = read(fd, buf, Constants::BUFLEN)) > 0)
	{
		kl += l;
		SHA512_Update(&ctx512, (unsigned char*)buf, l);
	}
	if (file)
		fclose(in);
	SHA512_End(&ctx512, (char*)buf);
	
	if (buf == NULL)
	{
		perror("sha512(const char* file) failed to process file");
		exit(-1);
	}
	else
	{
		string res((const char*)buf);
		return(res);
	}
}

string hashThem(const string& a, const string& b)
{
	return sha512(a + b); 
}

size_t uRandom(const size_t max, const unsigned short seed)
{
	boost::random::mt19937_64 rng(seed);
	boost::random::uniform_int_distribution<> N(1, max);
	return N(rng);
}

size_t uRandom(const size_t max, boost::random::mt19937_64& rng)
{
	boost::random::uniform_int_distribution<> N(1, max);
	return N(rng);
}

ProofOfWork powSolver(boost::random::mt19937_64& rng)
{
	ProofOfWork res;
	high_resolution_clock::time_point t0 = high_resolution_clock::now();
	double counter = 0;
	unsigned long long value;
	while (true)
	{
		boost::random::uniform_int_distribution<> N(1, Constants::MAXNONCE);
		size_t nonce = N(rng);					// current nonce
		counter += 1.0f;
		string s = sha512(to_string(nonce));
		string subs = s.substr(0, 16);			// pick the first 16 chars of the SHA512 hexadecimal, otherwise too large a number
		istringstream iss(subs);
		iss >> std::hex >> value;				// substring converted via hexadecimal to unsigned long long
		if (value > Constants::TARGET)			 
			continue;							// Bitcoin hash logic: reject 
		else
		{
			res.difficulty = counter / Constants::POWFACTOR;
			res.nonce = nonce;
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			duration<double> time_span = duration_cast<duration<double>>(t1 - t0);
			res.seconds = time_span.count();
			break;
		}
	}
	return res;
}
