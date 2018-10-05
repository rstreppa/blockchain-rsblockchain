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
	cout << "Type Ctrl-C to exit the program" << endl;

	// 2 Seed random number generator with unique server port
	boost::random::mt19937_64 rng(node.getPort());

	// 3 create server thread: server will listen for incoming Messages, 
	// decode them and Manage them, updating the BlockChain (via a static pointer to unique Node)
	Peer2Peer p2p(node, Constants::localhost);
	boost::thread s{ [&] {while (true) p2p.server(); } };

	// 4 genesis Block created after a random (small) sleep time in seconds
	// validated and placed on the BlockChain and sent over the network
	boost::random::uniform_int_distribution<> N(1, Constants::SLEEP_GENESIS);
	int x = N(rng);
	boost::this_thread::sleep_for(boost::chrono::seconds(x));
	Block genesis = node.makeGenesisBlock();
	node.updateChain(genesis);
	node.sendBlock(genesis);

	// The alternative solution in the next three lines does not work with member functions, need to review
	// auto f = std::bind(&Node::makeGenesisBlock, &node);	
	// auto sleepGenesis = makeSleepDecorator(Constants::SLEEP_GENESIS, rng, f);
	// Block genesis = sleepGenesis();

	// 5 create client thread to make Transactions with randomn sleep time in seconds
	// makeTransaction will update Balance, package Transaction in a Message and send to the network
	boost::thread t{ [&] {while (true) node.makeSleepTransaction(rng); } };

	// 6 create client thread to mine Blocks, naturally timed by the computational puzzle to be solved 
	// mineBlock will collect Transactions in a Block, update the BlockChain, package a Message and send over the Network
	boost::thread b{ [&] {while (true) node.mineBlock(rng); } };

	// 7 Join threads
	b.join();
	t.join();
	s.join();
	
	// 8 No need to delete pointer to unique Node (member of Peer2Peer) as we are exiting the program
	return 0;
}
// EOF