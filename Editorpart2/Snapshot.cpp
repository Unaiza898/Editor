#include "Snapshot.h"



void Snapshot::setCommand(char a) {

	command = a;

}



void Snapshot::setValue(string a) {

	value = a;

}



void Snapshot::set_uPos(Position a) {

	uPos = a;

}



char Snapshot::getCommand() {

	return command;

}



string Snapshot::getValue() {

	return value;

}



Position Snapshot::get_uPos() {

	return uPos;

}

//Snapshot.cpp