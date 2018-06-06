/*
 * SnakeClient.cpp
 *
 *  Created on: Nov 29, 2016
 *      Author: amirabbas
 */

#include "SnakeClient.h"

SnakeClient::SnakeClient() {
	srand((unsigned)time(0));

}

SnakeClient::~SnakeClient() {
}

void SnakeClient::loop()
{
	zmq::context_t context(1);
	zmq::socket_t zsocket(context, ZMQ_DEALER);
	zsocket.setsockopt(ZMQ_IDENTITY, socketId.c_str(),
			socketId.length());
	string adr = address;
	zsocket.connect(adr);
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

void SnakeClient::generateId() {
	socketId = "";
	socketId += char(rand()%256);
	socketId += char(rand()%256);
	socketId += char(rand()%256);
	socketId += char(rand()%256);
}

void SnakeClient::init()
{
	generateId();
	thread th1 = thread(&SnakeClient::loop, this);
	th1.detach();
}

void SnakeClient::send(zmq::socket_t & zsocket)
{
	string message = messageQueue[0];
	messageQueue.erase(messageQueue.begin());

	zmq::message_t msg_empty("", strlen(""));
	zsocket.send(msg_empty, ZMQ_SNDMORE);

	zmq::message_t msg(message.c_str(), message.size());
	zsocket.send(msg);

}
void SnakeClient::send(string message)
{
	messageQueue.push_back(message);
}
void SnakeClient::recv(zmq::socket_t & zsocket)
{
	string socketId = "", message;
	zmq::message_t msg;
	zsocket.recv(&msg);
	zsocket.recv(&msg);
	message = string(static_cast<char*>(msg.data()), msg.size());
	callback(message);

}
