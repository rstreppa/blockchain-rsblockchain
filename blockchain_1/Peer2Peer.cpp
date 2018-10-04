#include "Peer2Peer.h"
#include "Json.h"
#include <boost/thread.hpp>
#include <string>

using namespace std;


Peer2Peer::Peer2Peer(Node& node, std::string host)
{
	m_pNode = &node;			// initialized in the constructor but will stay initialized forever until deleted 
	m_msg	= Message();
	m_host	= host;
}

Peer2Peer::Peer2Peer(Node& node, const Message& msg, std::string host)
{
	m_pNode = &node;			// initialized in the constructor but will stay initialized forever until deleted 
	m_msg	= msg;
	m_host	= host;
}

Node* Peer2Peer::getPNode()
{
	return Peer2Peer::m_pNode;
}

bool Peer2Peer::deleteNode() 
{
	// TO BE IMPLEMENTED
	return true;
}

void Peer2Peer::client(const Message& msg, unsigned short to)
{
	// Jsonify Message
	string json = Json::write(msg);
	// Set Client and send Message
	if (0 == m_tcp.xpconnect(m_host.c_str()/*could be ipv6 address or hostname*/, to))
	{
		m_tcp.send(json.c_str(), Constants::MSGLEN);
	}
}

void Peer2Peer::client(unsigned short to)
{
	// Jsonify Message
	string json = Json::write(m_msg);
	
	// Set Client and send Message
	if (0 == m_tcp.xpconnect(m_host.c_str()/*could be ipv6 address or hostname*/, to))
	{
		m_tcp.send(json.c_str(), Constants::MSGLEN);
	}
}

void Peer2Peer::broadcast(const Message& msg) 
{
	// Jsonify Message
	string json = Json::write(msg);

	// update active Node port list
	vector<unsigned short> portList = m_pNode->getPortList();

	// Set Client and broadcast Message 
	for (auto to : portList)
	{
		// Set Client and send Message
		if (0 == m_tcp.xpconnect(m_host.c_str()/*could be ipv6 address or hostname*/, to))
		{
			m_tcp.send(json.c_str(), Constants::MSGLEN);
		}
	}
}

void Peer2Peer::broadcast()
{
	// Jsonify Internal Message
	string json = Json::write(m_msg);

	// update active Node port list
	vector<unsigned short> portList = m_pNode->getPortList();

	// Set Client and broadcast Message 
	for (auto to : portList)
	{
		// Set Client and send Message
		if (0 == m_tcp.xpconnect(m_host.c_str()/*could be ipv6 address or hostname*/, to))
		{
			m_tcp.send(json.c_str(), Constants::MSGLEN);
		}
	}
}

void Peer2Peer::server() 
{
	// 1) set up the server listening on Node own port
	boost::this_thread::interruption_point();		// not necessary now, but doesn't harm
	try
	{
		if (0 == m_tcp.pserv(m_host.c_str()/*could be ipv6 address*/, m_pNode->getPort()))
		{
			xxsocket cli = m_tcp.accept();
			char msg_rec[Constants::MSGLEN];
			cli.recv(msg_rec, Constants::MSGLEN);
			// cout << msg_rec << endl;

			// 2) convert received message from char[] to string (in fact Json)
			string jdoc(msg_rec);

			// 3) convert Json string to Message
			Message msg = Json::read_message(jdoc);

			// 2) Manage the Message from Node, the controller
			m_pNode->manageIncomingMsg(msg);
		}
	}
	catch (exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}