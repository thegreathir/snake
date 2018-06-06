#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <thread>
#include <atomic>
#include "SnakeClient.h"
#define CYCLE_TIME 150000

using namespace std;

SnakeClient snakeClient;
atomic_bool getFrame{true};
struct termios oldSettings, newSettings;

void handleKey () {
	tcgetattr( fileno( stdin ), &oldSettings );
	newSettings = oldSettings;
	newSettings.c_lflag &= (~ICANON & ~ECHO);
	tcsetattr( fileno( stdin ), TCSANOW, &newSettings );

	while(true) {
		char c;
		read( fileno( stdin ), &c, 1 );
		string message;
		message += c;
		snakeClient.send(message);
	}
}

int main(int ac, char** av) {


	string address = "tcp://";

	if (!av[1]) {
		cout << "address is empty!" << endl;
		return 0;
	}

	string name;
	cout << "Enter your name: ";
	cin >> name;

	address += av[1];

	snakeClient.setAddress(address);
	snakeClient.init();
	snakeClient.onFrame([](string message){
		cout << message << endl;
		getFrame.store(true);
	});


	signal(SIGINT,[](int){
		snakeClient.send("Disconnect");
		tcsetattr( fileno( stdin ), TCSANOW, &oldSettings );
		usleep(100000);
		exit(1);
	});

	snakeClient.send("Connect"+name);

	thread th = thread(handleKey);

	while(true) {
		if(getFrame.load()) {
			snakeClient.send("Frame");
			getFrame.store(false);
		}
		usleep(CYCLE_TIME);
	}

	th.detach();
}
