/**
Peer2Peer.h
Purpose:	class Peer-to-Peer creates communication channels among Nodes (underlying client-server)

@author rstreppa
@version 1.0 10/01/2018
*/

#pragma once
#include "Node.h"
#include "Message.h"
#include <string>
#include "xxsocket.h"
#include "Config.h"

using namespace std;
using namespace purelib::net;

class Peer2Peer {
private:
	static Node* m_pNode;		///< shared pointer would destroy the singleton Node	
	Message m_msg;				///< message to be received/transmitted
	std::string m_host;			///< host name, localhost by default for this app
	xxsocket m_tcp;				///< client/server socket 
public:
	/** @brief	Constructor with Node and default host, can change the state of Node */
	Peer2Peer(Node& node, std::string host = Constants::localhost);
	/** @brief	Constructor with Node, Message and default host, can change the state of Node */
	Peer2Peer(Node& node, const Message& msg, std::string host = Constants::localhost);
	/** @brief Destructor deletes everything except pointer to Node */
	~Peer2Peer() {}
	/** @brief	return the static pointer to Node */
	static Node* getPNode();
	/** @brief	destroy the one instance of pointer to singleton Node */
	static bool deleteNode();
	/** @brief	Jsonify Message to string and send to a given client specified by port */
	void client(const Message& msg, unsigned short to);
	/** @brief	Jsonify internal Message to string and send to a given client specified by port only */
	void client(unsigned short to);
	/** @brief	Jsonify Message to string and broadcast to network (all active ports except yourself) */
	void broadcast(const Message& msg);
	/** @brief	Jsonify internal Message to string and broadcast to network (all active ports except yourself) */
	void broadcast();
	/** @brief	run the server that listens on the Node port and takes action upon Jsonified Message reception */
	void server();
};


