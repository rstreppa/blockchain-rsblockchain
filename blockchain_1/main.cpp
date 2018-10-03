// date:	06/11/2018  
// author:	rstreppa

#include "Node.h"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/random.hpp>
#include<boost/random/uniform_int_distribution.hpp>
#include "Peer2Peer.h"

using namespace std;


int main(int argc, char* argv[])
{	
	// 1 create a unique Node and initalize it with balance and server port
	Node& node = Node::getInstance(1000000);
	cout << "Welcome! Node created with initial balance = " << node.getBalance() << endl;
	cout << "Node bound to server port = " << node.getPort() << endl;

	// 2 create thread group and seed random number generator with unique server port
	boost::thread_group tg;
	boost::random::mt19937_64 rng(node.getPort());

	// 3 create server thread 

	// 4 create thread for genesis Block




	// N delete instance of pointer to Node
	if (Peer2Peer::deleteNode())
		return 0;
	else
		return(1);
}
// EOF