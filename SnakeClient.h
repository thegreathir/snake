/*
 * SnakeClient.h
 *
 *  Created on: Nov 29, 2016
 *      Author: amirabbas
 */

#ifndef SNAKECLIENT_H_
#define SNAKECLIENT_H_

#include "zmq.hpp"
#include "zhelper.hpp"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <string>

using namespace std;

class SnakeClient {
private:
	string address;
	string socketId;
	vector <string> messageQueue;
	void loop();
	void recv(zmq::socket_t &);
	void send(zmq::socket_t &);

	void generateId();

	function <void(string)> callback;

public:
	SnakeClient();
	virtual ~SnakeClient();

	void init();
	void send(string);

	void setAddress(string address) {
		this->address = address;
	}

	void onFrame (function <void(string)> callback) {
		this->callback = callback;
	}
};

#endif /* SNAKECLIENT_H_ */
