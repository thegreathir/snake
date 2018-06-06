
#include <iostream>
#include <sys/time.h>
#include "World.h"

using namespace std;


int main() {

	World world;
	while (world.render()) {
		usleep(CYCLE_TYME);
	}

	cout << "GAME OVER" << endl;
}
