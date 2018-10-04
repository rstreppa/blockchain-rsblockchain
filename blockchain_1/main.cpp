// date:	06/11/2018  
// author:	rstreppa

#include "Node.h"
#include <iostream>
#include <boost/random.hpp>
#include "Peer2Peer.h"
#include <boost/thread.hpp>
#include "Decorator.h"
#include<boost/random/uniform_int_distribution.hpp>

using namespace std;


int main(int argc, char* argv[])
{	
	// 1 create a unique Node (singleton) and initalize it with balance, Node port randomly chosen among available
	Node& node = Node::getInstance(Constants::ACCOUNT);
	cout << "Welcome! Node created with initial balance = " << node.getBalance() << endl;
	cout << "Node bound to server port = " << node.getPort() << endl;

	// 2 Seed random number generator with unique server port
	boost::random::mt19937_64 rng(node.getPort());

	// 3 create server thread
	Peer2Peer p2p(node, Constants::localhost);
	boost::thread s{ [&] {while (true) p2p.server(); } };

	// 4 genesis Block created after a random (small) sleep time in seconds
	// validated and placed on the BlockChain and sent over the network
	boost::random::uniform_int_distribution<> N(1, Constants::SLEEP_GENESIS);
	int x = N(rng);
	boost::this_thread::sleep_for(boost::chrono::seconds(x));
	Block genesis = node.makeGenesisBlock();
	node.updateChain(genesis);
	// auto f = std::bind(&Node::makeGenesisBlock, &node);		// this solution does not work, need to review
	// auto sleepGenesis = makeSleepDecorator(Constants::SLEEP_GENESIS, rng, f);


	// Join threads
	s.join();
	
	// No need to delete pointer to unique Node as we are exiting the program
	return 0;
}
// EOF