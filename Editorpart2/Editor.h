#ifndef Editor_H
#define Editor_H

#include "LinkedList.h"
#include "Position.h"
#include "Snapshot.h"
#include <Windows.h>
#include "LinkedStack.h"
#include "selectionSort.cpp"
#include "BinarySearchTree.h"

#include <string>
#include <fstream>
#include <iostream>
#include <conio.h>
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define RESET   "\033[0m"       /* reset */

using namespace std;


class Editor {
public:
	Editor();//default constructor
	// takes input from the user 
	// takes the filename from the user
   //letting user to type lines and create a new file named by user
	Editor(string);
	//constructor which takes one string parameter
	~Editor(); // destructor
	Editor(const Editor&);// copy constructor
	void run();
	//get command from the user and processs it
	void displayLines(bool coloring); // displays the file
private:
	// to update the file and display on the console
	inline void update_display(Position);
	// go throught the file and save each keyword
	void set_keywords();
	// saving the datas before a command processing
	void save_undo(char command, string value, Position cursor);
	LinkedList <string> lines;
	string fName; // file name
	BinarySearchTree<string> keyWords;
	LinkedStack <Snapshot> undoStack;
};


#endif

