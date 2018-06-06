/*
 * SnakeServer.cpp
 *
 *  Created on: Nov 29, 2016
 *      Author: amirabbas
 */

#include "SnakeServer.h"

void SnakeServer::loop() {

	zmq::context_t context(1);
	zmq::socket_t zsocket(context, ZMQ_ROUTER);
	string adr = ADDRESS;
	zsocket.bind(adr);
	zmq::pollitem_t items[] = { { zsocket, 0, ZMQ_POLLIN, 0 } };
	while (true)
	{
		zmq::poll(items, 1, 5);
		if ( items[0].revents & ZMQ_POLLIN )
		{
			recv(zsocket);
		}
		if ( messageQueue.size() > 0 )
		{
			send(zsocket);
		}
	}
}

void SnakeServer::recv(zmq::socket_t & zsocket) {

	string socketId,message;
	zmq::message_t msg;
	zsocket.recv(&msg);
	socketId = string(static_cast<char*>(msg.data()), msg.size());
	zsocket.recv(&msg);
	zsocket.recv(&msg);
	message = string(static_cast<char*>(msg.data()), msg.size());
	callback(socketId,message);
}

void SnakeServer::send(zmq::socket_t & zsocket) {
	string message = messageQueue.begin()->second;
	string socketId = messageQueue.begin()->first;
	messageQueue.erase(messageQueue.begin());

	zmq::message_t msg_dealer_id(socketId.c_str(), socketId.size());
	zsocket.send(msg_dealer_id, ZMQ_SNDMORE);

	zmq::message_t msg_empty("", strlen(""));
	zsocket.send(msg_empty, ZMQ_SNDMORE);

	zmq::message_t msg(message.c_str(), message.size());
	zsocket.send(msg);
}

void SnakeServer::send(string socketId, string message)
{
	messageQueue.push_back(pair<string, string>(socketId, message));
}
void SnakeServer::init()
{
	thread th1 = thread(&SnakeServer::loop, this);
	th1.detach();
}

SnakeServer::SnakeServer() {
	// TODO Auto-generated constructor stub

}

SnakeServer::~SnakeServer() {
	// TODO Auto-generated destructor stub
}

