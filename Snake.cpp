/*
 * Snake.cpp
 *
 *  Created on: Nov 26, 2016
 *      Author: amirabbas
 */

#include "Snake.h"

Snake::Snake(MoveType m,int x,int y,int _id,Color _color,string _name) {
	body.push_back( Point (x,y));
	dir = m;

	int xx,yy;

	switch (m) {
		case UP:
			yy = 1;
			xx = 0;
			break;
		case DOWN:
			yy = -1;
			xx = 0;
			break;
		case RIGHT:
			xx = -1;
			yy = 0;
			break;
		case LEFT:
			xx = 1;
			yy = 0;
			break;
		default:
			break;
	}

	body.push_back( Point (x+xx,y+yy));
	body.push_back( Point (x+2*xx,y+2*yy));

	charge = 0;
	id = _id;
	color = _color;
	name = _name;
}

string Snake::getName () {
	return name;
}

Snake::~Snake() {
}

bool Snake::isMoveValid (MoveType mv) {

	switch (mv) {

		case LEFT:

			return !(body[0].second == body[1].second &&
					 body[1].first < body[0].first);

		case RIGHT:

			return !(body[0].second == body[1].second &&
					 body[1].first > body[0].first);
		case UP:

			return !(body[0].first == body[1].first &&
					 body[1].second < body[0].second);
		case DOWN:

			return !(body[0].first == body[1].first &&
					 body[1].second > body[0].second);

		case CONTINUE:
			return true;

		default:
			return false;
	}
}

void Snake::move(MoveType mv) {
	switch (mv) {
		case LEFT:

			if(isMoveValid(LEFT)) {

				Point head (body[0].first - 1 ,body[0].second);
				body.insert(body.begin(),head);
				dir = LEFT;

			}
			else {
				move(dir);
				return;
			}
			break;

		case RIGHT:

			if(isMoveValid(RIGHT)) {

				Point head (body[0].first + 1 ,body[0].second);
				body.insert(body.begin(),head);
				dir = RIGHT;

			}
			else{
				move(dir);
				return;
			}
			break;

		case UP:

			if(isMoveValid(UP)) {

				Point head (body[0].first ,body[0].second - 1);
				body.insert(body.begin(),head);
				dir = UP;

			}
			else{
				move(dir);
				return;
			}
			break;

		case DOWN:

			if(isMoveValid(DOWN)) {

				Point head (body[0].first ,body[0].second + 1);
				body.insert(body.begin(),head);
				dir = DOWN;

			}
			else{
				move(dir);
				return;
			}
			break;

		case CONTINUE:

			if(isMoveValid(CONTINUE)) {
				move(dir);
			}
			return;
			break;


	}

	if(charge == 0)
		body.pop_back();
	if(charge)
		charge--;
}


bool Snake::operator == (Snake snake) {
	return this->id == snake.id;
}

Color Snake::getColor() {
	return color;
}

void Snake::add(int _c){
	charge += _c;
}

vector <Point> Snake::getBody() {
	return body;
}


Point Snake::getHead() {
	return *body.begin();
}

MoveType Snake::getDir () {
	return dir;
}

bool Snake::containBody (Point p){

	for ( vector<Point>::iterator it = body.begin();it != body.end(); ++it) {
		if(it->first == p.first && it->second == p.second)
			return true;
	}

	return false;
}
int Snake::getLength() {
	return body.size();
}

int Snake::getId() {
	return id;
}






