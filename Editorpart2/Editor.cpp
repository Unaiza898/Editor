#include "Editor.h"
#define WHITE   "\033[37m"      /* White */
enum key { escKey = 27, enterKey = '\r', backspaceKey = 8 };
string mode; // this tells the user which mode are they in.
void placeCursorAt(Position coordinate) {
	COORD coord;
	coord.X = coordinate.getX();
	coord.Y = coordinate.getY();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Default constructor.
//Let user to create a new text file and named by user
Editor::Editor() {
	//give name to number
	enum key { escKey = 27, enterKey = '\r', backspaceKey = 8 };
	set_keywords();
	Position Cursor(0, 28);
	placeCursorAt(Cursor);
	cout << "-------------------------------------------" << endl;
	cout << BOLDGREEN << "Insert Mode. Press ESC to quit!\n" << RESET;
	Cursor.setY(0);
	placeCursorAt(Cursor);

	string line = "";	//line is using to restore all the key for one line that user enter
	int counter = 1;	//count current line
	char word;
	for (;;) {
		// get the key from the user
		word = _getch();
		switch (word) {
			// if the key user enter is Esc which is to exit insert mode
		case escKey: {
			lines.insert(counter, line);
			counter++;
			run();
			break;
		}
				   // if the key user enter is Enter which means the line end
		case enterKey: {
			cout << endl;
			lines.insert(counter, line);
			counter++;
			line = "";
			break;
		}
					 //if the key user enter is backspace
		case backspaceKey: {
			if (line.length() > 0) {
				string temp = "";
				for (int i = 0; i < line.length() - 1; i++) {
					temp += line[i];
				}
				line = temp;
				system("CLS");

				Cursor.setY(28);
				placeCursorAt(Cursor);
				cout << "-------------------------------------------" << endl;
				cout << BOLDGREEN << "Insert Mode. Press ESC to quit!\n" << RESET;
				Cursor.setY(0);
				placeCursorAt(Cursor);

				displayLines(false);
				cout << line;
				break;
			}
			//for anyotherkey we just display it and append it to line.
		default: {
			cout << word;
			line += word;
		}
		}
		}
	}
}


Editor::Editor(string fileName) {
	// open the file that the user specifies
	set_keywords();
	ifstream inFile(fileName);
	string line;
	fName = fileName;
	int counter = 1;
	//check if the file open
	if (inFile.is_open()) {
		//file open
		//get all the lines in the file and store it to the linklist class
		while (!inFile.eof()) {
			getline(inFile, line);
			lines.insert(counter, line);
			counter++;
		}
		inFile.close();
	}
	else {
		//file cannot open
		cout << "Error,the file doesn't exist\n";
		exit(0);
	}
}

Editor::~Editor() {
}

Editor::Editor(const Editor& si) { //copy
}

void Editor::run() {
	Position Cursor(0, 0);
	placeCursorAt(Cursor);
	//get input from the user but doesn't show
	//no need to press enter for the user
	for (;;) {
		char directive = _getch();
		switch (directive) {
		case ':': {
			Cursor.setY(30);
			Cursor.setX(0);
			update_display(Cursor);
			
			//placeCursorAt(Cursor);
			cout<< WHITE << directive;// display :

			
			char directive2 = _getch();
			cout << directive2;//display command
			switch (directive2) {
				//   :0 goes to the beginning of the first line(colon as before puts the cursor at the end of the user screen)
			case '0': {
				Cursor.setX(0);
				Cursor.setY(0);
				update_display(Cursor);
				break;
			}
					//   :$ goes to the end of the current line
			case '$': {
				Cursor.setX(0);
				Cursor.setY(lines.getLength() - 1);
				update_display(Cursor);
				break;
			}
					//quit
			case 'q':
				//let the cursor at the bottom
				Cursor.setY(30);
				Cursor.setX(0);
				update_display(Cursor);

				exit(0);
				//save the file

			case 'w': {
				Cursor.setY(30);
				Cursor.setX(0);
				update_display(Cursor);
				cout << directive;// display :
				cout << directive2;//display command
				cout << "\t";
				ofstream outFile;
				string fileName;
				getline(cin, fileName);
				fName = fileName;
				outFile.open(fileName);
				for (int i = 1; i <= lines.getLength(); i++) {
					if (i == lines.getLength())
						outFile << lines.getEntry(i);
					else
						outFile << lines.getEntry(i) << endl;
				}
				outFile.close();
				exit(1);
			}
			}
			break;
		}
				// when user enters i it goes to the insert mode
		case'i': {
			mode = "insert";
			Position Cursortrack(Cursor);
			Cursor.setY(28);
			placeCursorAt(Cursor);
			cout << "-------------------------------------------" << endl;
			cout << BOLDGREEN << "Insert Mode. Press ESC to quit!\n" << RESET;
			Cursor.setY(0);
			placeCursorAt(Cursortrack);


			string line = "";	//line is using to restore all the key for one line that user enter
			int counter = 1;	//count current line
			char word;
			for (;;) {
				// get the key from the user
				word = _getch();
				switch (word) {
					// if the key user enter is Esc which is to exit insert mode
				case escKey: {
					mode = "default";
					update_display(Cursor);
					run();
					break;
				}

		
						   //for anyotherkey we just display it and append it to line.
				default: {


					string newline = "";
					string oldLine = lines.getEntry(Cursor.getY() + 1);
					// add the word where the cursor is at and rewrite the line.
					for (int i = 0; i <= oldLine.length(); i++) {
						if (i == Cursor.getX())
							newline += word;

						newline += lines.getEntry(Cursor.getY() + 1)[i];
					}
					lines.replace(Cursor.getY() + 1, newline);
					Cursor.setX(Cursor.getX() + 1); // this move the cursor to the right

					update_display(Cursor); // updates the display with the cursor and changes made
					placeCursorAt(Cursor);
			


				}
				}
				
			}






		}
				//delete the line
		case 'd': {
			directive = _getch();
			if (directive == 'd') {
				save_undo(directive, lines.getEntry(Cursor.getY() + 1), Cursor);
				lines.remove(Cursor.getY() + 1);
				update_display(Cursor);
			}
			break;
		}
				//delete the first letter fo the line
		case 'x': {
			save_undo(directive, lines.getEntry(Cursor.getY() + 1), Cursor);
			string newLine = "";
			string oldLine = lines.getEntry(Cursor.getY() + 1);
			for (int i = 0; i < oldLine.length(); i++) {
				if (i == Cursor.getX())
					continue;
				newLine += lines.getEntry(Cursor.getY() + 1)[i];
			}
			lines.replace(Cursor.getY() + 1, newLine);
			update_display(Cursor);
			break;
		}
				//move the cursor up
		case 'k': {
			if (Cursor.getY() != 0) {
				string line = lines.getEntry(Cursor.getY() + 1);
				string lastLine = lines.getEntry(Cursor.getY());
				if (Cursor.getX() > lastLine.length()) {
					Cursor.setX(lastLine.length() - 2);
				}
				Cursor.setY(Cursor.getY() - 1);
				placeCursorAt(Cursor);
			}
			break;
		}
				//move the cursor down
		case 'j': {
			if (Cursor.getY() + 1 < lines.getLength()) {
				string line = lines.getEntry(Cursor.getY() + 1);
				string nextLine = lines.getEntry(Cursor.getY() + 2);
				if (Cursor.getX() > nextLine.length()) {
					Cursor.setX(nextLine.length() - 2);
				}
				Cursor.setY(Cursor.getY() + 1);
				placeCursorAt(Cursor);
			}
			break;
		}
				//move the cursor right
		case 'l': {
			string line = lines.getEntry(Cursor.getY() + 1);
			if (Cursor.getX() <= line.length() - 2) {
				Cursor.setX(Cursor.getX() + 1);
				placeCursorAt(Cursor);
			}
			break;
		}
				//move the cursor left
		case 'h': {
			if (Cursor.getX() != 0) {
				Cursor.setX(Cursor.getX() - 1);
				placeCursorAt(Cursor);
			}
			break;
		}
				//undo
		case 'u': {
			//check if the previous command
			if (!undoStack.isEmpty()) {
				Snapshot undo;
				undo = undoStack.peek();
				switch (undo.getCommand())
				{
					//when the previous command is d
				case 'd': {
					Cursor = undo.get_uPos();
					string line;
					for (int i = 0; i < undo.getValue().length(); i++) {
						if (undo.getValue()[i] == '\n') {
							lines.insert(Cursor.getY() + 1, line);
							line = "";
						}
						else
							line += undo.getValue()[i];
					}
					undoStack.pop();
					update_display(Cursor);
					break;
				}
						//when the previous command is x
				case 'x': {
					Cursor = undo.get_uPos();
					lines.replace(Cursor.getY() + 1, undo.getValue());
					undoStack.pop();
					update_display(Cursor);
					break;
				}
				}
			}
			else {
				break;
			}
		}
				// goes to the beginning of the current line
		case '0': {
			Cursor.setX(0);
			update_display(Cursor);
			break;
		}
				//  goes to the end of the current line
		case '$': {
			Cursor.setX(lines.getEntry(Cursor.getY() + 1).length() - 1);
			update_display(Cursor);
			break;
		}


				//case positive numbers 1-9
				// using for inplement Nj Nk Nh Nl Nx Ndd where N is the positive number
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			int num = directive - '0';
			char directive2 = _getch();
			switch (directive2) {
				// this will delete N words
			case 'x': {
				save_undo(directive2, lines.getEntry(Cursor.getY() + 1), Cursor);
				string newLine = "";
				string oldLine = lines.getEntry(Cursor.getY() + 1);
				for (int i = 0; i < oldLine.length(); i++) {
					if (i >= Cursor.getX() && i < Cursor.getX() + num)
						continue;
					newLine += lines.getEntry(Cursor.getY() + 1)[i];
				}
				lines.replace(Cursor.getY() + 1, newLine);
				update_display(Cursor);
				break;
			}
					// this will delete N lines
			case 'd': {
				directive2 = _getch();
				if (directive2 == 'd') {
					string removeline;
					for (int i = num; i > 0; i--) {
						removeline += lines.getEntry(Cursor.getY() + i);
						removeline += '\n';
						lines.remove(Cursor.getY() + i);
					}
					save_undo(directive2, removeline, Cursor);
					update_display(Cursor);
				}
				break;
			}
					//move n unit up
			case 'k': {
				if (Cursor.getY() < num) {
					num = Cursor.getY();
				}
				string line = lines.getEntry(Cursor.getY() + 1);
				string lastLine = lines.getEntry(Cursor.getY() + 1 - num);
				if (Cursor.getX() > lastLine.length()) {
					Cursor.setX(lastLine.length() - 2);
				}
				Cursor.setY(Cursor.getY() - num);
				placeCursorAt(Cursor);
				break;
			}
					//move the cursor n unit down
			case 'j': {
				if (Cursor.getY() + num >= lines.getLength() - 1) {
					num = lines.getLength() - Cursor.getY() - 1;
				}
				if (Cursor.getY() + 1 < lines.getLength()) {
					string line = lines.getEntry(Cursor.getY() + 1);
					string nextLine = lines.getEntry(Cursor.getY() + 1 + num);
					if (Cursor.getX() > nextLine.length()) {
						Cursor.setX(nextLine.length() - 2);
					}
					Cursor.setY(Cursor.getY() + num);
					placeCursorAt(Cursor);
				}
				break;
			}
					//move the cursor n unit right
			case 'l': {
				string line = lines.getEntry(Cursor.getY() + 1);
				if (Cursor.getX() <= line.length() - 2) {
					if (Cursor.getX() + num >= line.length() - 2) {
						num = line.length() - 1 - Cursor.getX();
					}
					Cursor.setX(Cursor.getX() + num);
					placeCursorAt(Cursor);
				}
				break;
			}
					//move the cursor n unit left
			case 'h': {
				if (Cursor.getX() != 0) {
					if (Cursor.getX() - num <= 0)
						Cursor.setX(0);
					else
						Cursor.setX(Cursor.getX() - num);
					placeCursorAt(Cursor);
				}
				break;
			}
			}
			break;
		}
		}
	}
}

//binarySearch function
int binarySearch(const string anArray[], int first, int last, string target) {
	int index;
	if (first > last)
		index = -1; // target not in original array
	else {
		// If target is in anArray,
		// anArray[first] <= target <= anArray[last]
		int mid = first + (last - first) / 2;
		if (target == anArray[mid])
			index = mid; // target found at anArray[mid]
		else if (target < anArray[mid])
			// Point X
			index = binarySearch(anArray, first, mid - 1, target);
		else
			// Point Y
			index = binarySearch(anArray, mid + 1, last, target);
	} // end if
	return index;
} // end binarySearch

void colorText(int value) {
	COORD coord;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hConsole);
	if (value == 0)
		SetConsoleTextAttribute(hConsole, 15);
	else
		SetConsoleTextAttribute(hConsole, 9);
}

void Editor::displayLines(bool coloring) {
	int position;
	string nextLine, nextWord, line;
	if (coloring == true) {
		// goes through each line in the linked list 
		for (position = 1; position <= lines.getLength(); position++) {
			nextLine = lines.getEntry(position);

			int i = 0;
			while (i < nextLine.length()) {
				string word;
				// isolate a word at a time (can contain underscores)
				if (isalpha(nextLine[i])) {
					while (isalpha(nextLine[i]) || nextLine[i] == '_') {
						word += nextLine[i];
						i++;
					}
					if (keyWords.contains(word))  //found
						colorText(0);
					else
						colorText(1);
					cout << word;
				}
				else {
					colorText(1);
					cout << nextLine[i];
					i++;
				}
			}
			cout << endl;
		}
	}
	else {
		for (int i = 1; i <= lines.getLength(); i++) {
			cout << lines.getEntry(i) << endl;
		}
	}
} // end displayLines


inline void Editor::update_display(Position Cursor) {
	//to update the display of console
	system("CLS");
	displayLines(true);
	Position Cursortrack(Cursor);
	if (mode == "insert")
	{
		Cursor.setX(0);
		Cursor.setY(28);
		placeCursorAt(Cursor);
		cout << "-------------------------------------------" << endl;
		cout << BOLDGREEN << "Insert Mode. Press ESC to quit!\n" << RESET;
		Cursor.setY(0);
		
	}
	placeCursorAt(Cursortrack);
}



void Editor::set_keywords() { // open the file save the words
	fstream inFile;
	inFile.open("keyword.txt");
	if (!inFile) {
		//check if the file got open
		cout << "can't open the keyword file";
		exit(0);
	}
	while (!inFile.eof()) {
		string b;
		inFile >> b;
		keyWords.add(b);
	}
	inFile.close();
}

//Function using to save the all datas before the command processes
void Editor::save_undo(char command, string value, Position cursor) {
	Snapshot temp;
	temp.setCommand(command);
	temp.setValue(value);
	temp.set_uPos(cursor);
	undoStack.push(temp);
}