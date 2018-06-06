/*
 * Snake.h
 *
 *  Created on: Nov 26, 2016
 *      Author: amirabbas
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include "Point.h"
#include "Config.h"

#include <vector>

enum Color {
	brown,
	red,
	green,
	blue,
	gold,
	purple,
	white,
	empty
};



enum MoveType {
	UP,
	RIGHT,
	DOWN,
	LEFT,
	CONTINUE
};

using namespace std;

class Snake {
private:
	vector <Point> body;
	MoveType dir;
	int charge;
	int id;
	Color color;
	string name;
public:
	void       move        (MoveType);
	bool       isMoveValid (MoveType);
	Point      getHead     ();
	MoveType   getDir      ();
	bool       containBody (Point);
	void       add         (int);
	int        getLength   ();
	int        getId       ();
	Color      getColor    ();
	string     getName     ();
	vector <Point> getBody ();

	Snake(MoveType,int,int,int,Color,string);
	Snake(){
		dir = CONTINUE;
		charge = 0;
		id = -1;
		color = empty;
		name = "no name";
	}


	virtual ~Snake();

	bool operator == (Snake);

};

#endif /* SNAKE_H_ */
