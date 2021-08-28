
#ifndef Snapshot_H

#define Snapshot_H



#include <iostream>

#include <string>

#include "Position.h"

using namespace std;



class Snapshot {

public:

	void setCommand(char);

	void setValue(string);

	void set_uPos(Position);

	char getCommand();

	string getValue();

	Position get_uPos();



private:

	char command;

	string value;

	Position uPos;

};



#endif

//Snapshot.h