#ifndef Position_H
#define Position_H
// This class help access the cursor coordinates
class Position {
public:
	Position();
	Position(int, int);
	void setX(int); // set the value of  x coordinate
	void setY(int); // set the value of the y coordinate
	int getX() const;  // returns the value x coordinate
	int getY() const; // returns the value y coordinate
	void display() const;
private:
	int x;
	int y;
};
#endif