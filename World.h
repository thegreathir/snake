/*
 * World.h
 *
 *  Created on: Nov 26, 2016
 *      Author: amirabbas
 */

#ifndef WORLD_H_
#define WORLD_H_

#include "Snake.h"
#include "Config.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "SnakeServer.h"
#include <unordered_map>

using namespace std;

class World {
private:

	vector<Snake> snakes;

	char **map ;
	Color color[WORLD_WIDTH][WORLD_HEIGHT];

	int state;
	Point score;
	int scoreVal;

	int idC;
	int boost;
	bool paused;

	string frame;

	void clearMap ();
	void show();
	void setUp();
	bool isHeadOut(Point);
	bool end(Snake);
	MoveType moveHandler (Snake);
	bool catchedScore (Snake);
	void generateScore();
	bool containAnySnake(Point);
	bool containAnyOtherSnakes(Snake);
	string getColor(Color);
	Snake  newSnake(string);
	Snake  newSnake(int,Color,string);
	Color generateColor();

	MoveType parseMessage (char);

	SnakeServer snakeServer;

	unordered_map <int,string> idMap;
	unordered_map <string,int> socketMap;
	unordered_map <int,MoveType> moveMap;
	vector<Snake> remoteSnakes;


public:
	World();
	bool render();
	virtual ~World();
};

#endif /* WORLD_H_ */
