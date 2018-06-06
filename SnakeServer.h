/*
 * SnakeServer.h
 *
 *  Created on: Nov 29, 2016
 *      Author: amirabbas
 */

#ifndef SNAKESERVER_H_
#define SNAKESERVER_H_

#include "zmq.hpp"
#include "zhelper.hpp"
#include <thread>
#include <string>
#include "Config.h"

using namespace std;



class SnakeServer {
private:
	void loop();
	vector<pair<string, string>> messageQueue;
	void send(zmq::socket_t &);
	void recv(zmq::socket_t &);
	function <void(string,string)> callback;
public:
	SnakeServer();
	virtual ~SnakeServer();
	void send(string, string);
	void init();

	void onFrame (function <void(string,string)> callback) {
		this->callback = callback;
	}
};

#endif /* SNAKESERVER_H_ */
