#include "Position.h"
#include <iostream>
using namespace std;

Position::Position() {
	x = 0;
	y = 0;
}

Position::Position(int x, int y) {
	this->x = x;
	this->y = y;
}

void Position::setX(int x) {
	this->x = x;
}

void Position::setY(int y) {
	this->y = y;
}

int Position::getX() const {
	return x;
}

int Position::getY() const {
	return y;
}

void Position::display() const {
	cout << "(" << x << "," << y << ")" << endl;
}