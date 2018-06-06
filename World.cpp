/*
 * World.cpp
 *
 *  Created on: Nov 26, 2016
 *      Author: amirabbas
 */

#include "World.h"

void World::setUp() {
	map = new char*[WORLD_WIDTH];
	for (int i = 0;i<WORLD_WIDTH;i++) {
			map[i] = new char[WORLD_HEIGHT];
	}
}

void World::clearMap () {
	for (int i = 0;i<WORLD_WIDTH;i++) {
		for (int j = 0;j<WORLD_HEIGHT;j++) {

			if ( i == 0               ||
				 i == WORLD_WIDTH -1  ||
				 j == 0               ||
				 j == WORLD_HEIGHT -1) {
				map[i][j] = '#';
				color[i][j] = brown;
			}
			else
				map[i][j] = ' ';
		}
	}

	map[score.first][score.second] = '*';
	color[score.first][score.second] = purple;
	if(scoreVal == 10)
		color[score.first][score.second] = gold;

	for(vector<Snake>::iterator snake = snakes.begin();snake != snakes.end();snake++) {

		vector<Point> body = snake->getBody();

		for ( vector<Point>::iterator it = body.begin();it != body.end(); ++it) {
			if(it == body.begin())
				map[it->first][it->second] = 'x';
			else
				map[it->first][it->second] = 'o';

			color[it->first][it->second] = snake->getColor();
		}
	}

	//add remote snakes
	for(vector<Snake>::iterator remoteSnake = remoteSnakes.begin();remoteSnake != remoteSnakes.end();remoteSnake++) {

		vector<Point> body = remoteSnake->getBody();

		for ( vector<Point>::iterator it = body.begin();it != body.end(); ++it) {
			if(it == body.begin())
				map[it->first][it->second] = 'x';
			else
				map[it->first][it->second] = 'o';

			color[it->first][it->second] = remoteSnake->getColor();
		}
	}

}


bool World::isHeadOut(Point head) {
	int i = head.first,j = head.second;
	return ( i == 0               ||
			 i == WORLD_WIDTH -1  ||
			 j == 0               ||
			 j == WORLD_HEIGHT -1);
}

bool World::catchedScore(Snake snake) {
	if(snake.getHead() == score)
		return true;
	return false;
}


bool World::render() {


	for (auto remoteSnake = remoteSnakes.begin();remoteSnake != remoteSnakes.end();remoteSnake++) {
		if(idMap[remoteSnake->getId()] == "DISCONNECTED") {
			remoteSnakes.erase(remoteSnake);
			break;
		}
	}

	if(paused)
		return true;

	for (auto remoteSnake = remoteSnakes.begin();remoteSnake != remoteSnakes.end();remoteSnake++) {

		if (isHeadOut(remoteSnake->getHead()))
			*remoteSnake = newSnake(remoteSnake->getId(),remoteSnake->getColor(),remoteSnake->getName());

		if (containAnyOtherSnakes(*remoteSnake))
			*remoteSnake = newSnake(remoteSnake->getId(),remoteSnake->getColor(),remoteSnake->getName());

		remoteSnake->move(moveMap[remoteSnake->getId()]);
		if(moveMap[remoteSnake->getId()] != CONTINUE)
			moveMap[remoteSnake->getId()] = CONTINUE;

		if(end(*remoteSnake))
			*remoteSnake = newSnake(remoteSnake->getId(),remoteSnake->getColor(),remoteSnake->getName());

		if(catchedScore(*remoteSnake)) {
			remoteSnake->add(scoreVal);
			generateScore();
		}
	}


	//handle move

	vector<vector<Snake>::iterator> losers;

	for(vector<Snake>::iterator it = snakes.begin();it != snakes.end();it++) {
		if(end(*it)) {
			losers.push_back(it);
		}
		else{
			it->move(moveHandler(*it));
		}

	}

	for(vector<vector<Snake>::iterator>::iterator it = losers.begin(); it != losers.end();it ++) {
		snakes.erase(*it);
	}

	for(unsigned int i=0;i<losers.size();i++)
		snakes.push_back(newSnake(idC++,red,"AI"));

	for(vector<Snake>::iterator it = snakes.begin();it != snakes.end();it++) {
		if(catchedScore(*it)) {
			it->add(scoreVal);
			generateScore();
		}
	}

	//update map
	clearMap();
	show();
	state++;

	return true;
}

void World::show() {

	string buffer;

	buffer += "\033[2J";
	for (int i = 0;i<WORLD_HEIGHT;i++) {
		for (int j = 0;j<WORLD_WIDTH;j++) {
			buffer += getColor(color[j][i]);
			buffer += map[j][i];
			buffer += "\033[0m ";
		}
		buffer += "\n";
	}

	vector <pair<int,string>> results;

	for(vector<Snake>::iterator it = snakes.begin();it != snakes.end();it++) {

		string buffer = "";
		buffer += getColor(it->getColor());
		buffer += it->getName();
		buffer += ":";
		buffer += to_string(it->getLength());
		buffer += "\033[0m";
		for(unsigned int i = 0;i< 12 - it->getName().size();i++)
			buffer += " ";
		results.push_back(pair<int,string>(it->getLength(),buffer));
	}


	for(vector<Snake>::iterator it = remoteSnakes.begin();it != remoteSnakes.end();it++) {

		string buffer = "";
		buffer += getColor(it->getColor());
		buffer += it->getName();
		buffer += ":";
		buffer += to_string(it->getLength());
		buffer += "\033[0m";
		for(unsigned int i = 0;i< 12 - it->getName().size();i++)
			buffer += " ";
		results.push_back(pair<int,string>(it->getLength(),buffer));
	}

	sort (results.begin(),results.end(),[](pair<int,string> p1, pair<int,string> p2) {
		return p1.first > p2.first;
	});

	for (auto it = results.begin(); it != results.end();it++) {
		buffer += it->second;
	}

	frame = buffer;

	cout << buffer << endl;
}

World::World() {
	srand((unsigned)time(0));
	setUp();
	generateScore();
	state = 0;
	snakes.push_back(Snake(UP,WORLD_WIDTH/2,5,0,red,"AI1"));
	snakes.push_back(Snake(DOWN,WORLD_WIDTH/2,WORLD_HEIGHT-5,1,red,"AI2"));
	idC = 3;
	boost = 0;
	paused = false;

	snakeServer.onFrame([&](string socketId ,string message) {
		if(message.substr(0,7) == "Connect") {
			idMap[idC] = socketId;
			socketMap[socketId] = idC;
			moveMap[idC] = CONTINUE;
			string name = message.substr(7);
			remoteSnakes.push_back(newSnake(name));
		}
		else if (message == "Disconnect") {
			idMap[socketMap[socketId]] = "DISCONNECTED";
		}
		else if (message == "Frame") {
			snakeServer.send(socketId,frame);
		}
		else {
			moveMap[socketMap[socketId]] = parseMessage(message[0]);
		}
	});

	snakeServer.init();

	frame = "";

}



MoveType World::moveHandler(Snake snake) {
	MoveType dir = snake.getDir();
	MoveType result;
	Point head = snake.getHead();

	//getting score

	if(head.first == score.first) {
		if (head.second < score.second and dir != UP)
			dir = DOWN;
		if (head.second > score.second and dir != DOWN)
			dir = UP;
	}

	if(head.second == score.second) {
		if (head.first < score.first and dir != LEFT)
			dir = RIGHT;
		if (head.first > score.first and dir != RIGHT)
			dir = LEFT;
	}


	// not going out

	do{
		head = snake.getHead();
		switch (dir) {
			case LEFT:
				head = Point(head.first -1,head.second);
				result = LEFT;
				dir = UP;
				break;
			case UP:
				head = Point(head.first,head.second - 1);
				result = UP;
				dir = RIGHT;
				break;
			case RIGHT:
				head = Point(head.first+1,head.second );
				result = RIGHT;
				dir = DOWN;
				break;
			case DOWN:
				head = Point(head.first,head.second + 1);
				result = DOWN;
				dir = LEFT;
				break;
			default:
				break;
		}

	}while(isHeadOut(head) || containAnySnake(head));

	return result;


}


void World::generateScore() {
	int x,y;
	do {
		x = rand()%(WORLD_WIDTH-2)  + 1;
		y = rand()%(WORLD_HEIGHT-2) + 1;
		score = Point(x,y);

	}while(containAnySnake(score));

	int r = rand()%4;
	if(r == 0)
		scoreVal = 10;
	else
		scoreVal = 1;
}

Snake World::newSnake(int _id,Color color,string name) {
	int x,y;
		Point pos,pos2,pos3;
		int xx,yy;

		MoveType move;
		int dir = rand()%4;
		switch (dir) {
			case 0:
				move = UP;
				yy = 1;
				xx = 0;
				break;
			case 1:
				move = DOWN;
				yy = -1;
				xx = 0;
				break;
			case 2:
				move = RIGHT;
				xx = -1;
				yy = 0;
				break;
			case 3:
				move = LEFT;
				xx = 1;
				yy = 0;
				break;
			default:
				break;
		}

		do {
				x = rand()%(WORLD_WIDTH-6)  + 2;
				y = rand()%(WORLD_HEIGHT-6) + 2;
				pos = Point(x,y);
				pos2 = Point(x+xx,y+yy);
				pos3 = Point(x+2*xx,y+2*yy);

		}while(containAnySnake(pos) or containAnySnake(pos2) or containAnySnake(pos3));

		return Snake(move,x,y,_id,color,name);

}

Snake World::newSnake(string name) {
	return newSnake(idC++,generateColor(),name);
}


World::~World() {
	for (int i = 0;i<WORLD_WIDTH;i++) {
			delete [] map[i];
	}
	delete [] map;
}

bool World::end(Snake snake) {
	int x = snake.getHead().first  ,
		y = snake.getHead().second ;
	return (
				(isHeadOut(Point(x+1,y)) || containAnySnake(Point(x+1,y))) &&
				(isHeadOut(Point(x-1,y)) || containAnySnake(Point(x-1,y))) &&
				(isHeadOut(Point(x,y+1)) || containAnySnake(Point(x,y+1))) &&
				(isHeadOut(Point(x,y-1)) || containAnySnake(Point(x,y-1)))
			);
}


bool World::containAnySnake(Point p) {

	for(vector<Snake>::iterator it = snakes.begin();it != snakes.end();it++) {
		if(it->containBody(p))
			return true;
	}
	for(vector<Snake>::iterator it = remoteSnakes.begin();it != remoteSnakes.end();it++) {
			if(it->containBody(p))
				return true;
	}
	return false;
}

MoveType World::parseMessage(char key) {
	switch (key) {
		case 'w':
			return UP;
		case 's':
			return DOWN;
		case 'a':
			return LEFT;
		case 'd':
			return RIGHT;
		default:
			return CONTINUE;
	}
}

bool World::containAnyOtherSnakes(Snake s)
{
	for(vector<Snake>::iterator it = snakes.begin();it != snakes.end();it++) {
		if(it->containBody(s.getHead()))
			return true;
	}
	for(vector<Snake>::iterator it = remoteSnakes.begin();it != remoteSnakes.end();it++) {
		if(it->getId() != s.getId() and it->containBody(s.getHead()))
			return true;
	}

	vector<Point> body = s.getBody();
	for(unsigned i =1;i<body.size();i++)
		if(s.getHead()==body[i])
			return true;
	return false;
}

string World::getColor(Color c) {
	switch (c) {
		case red:
			return "\033[1;31m";
		case brown:
			return "\033[1;36m";
		case green:
			return "\033[1;32m";
		case blue:
			return "\033[1;34m";
		case gold:
			return "\033[1;33m";
		case purple:
			return "\033[1;35m";
		case white:
			return "\033[1;37m";
		default:
			return "";
	}
}

Color World::generateColor() {
	int r = idC%6;
	switch (r) {
		case 0:
			return brown;
		case 1:
			return green;
		case 2:
			return blue;
		case 3:
			return gold;
		case 4:
			return purple;
		case 5:
			return white;
		default:
			return empty;

	};
}


