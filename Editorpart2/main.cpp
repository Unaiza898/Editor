#include "Editor.h"
/*
This program allows the user to open the editor,
creates a new file, and opens an existing file that user specifies
The user can exit the file by typing:q
creating a new file by typing:w
*/

void main(int argc, const char* argv[]) {
	cout << "(press 'Enter' if you open file which wrote behinde 'si')\n";
	cout << "(Or enter any other keys to start a new one):";
	// User presses Enter to deal with the file that the user specifies.
	if (cin.get() == '\n') {
		system("CLS");
		Editor si(argv[1]);
		si.displayLines(true);
		si.run();
	}
	// Enter any other keys to allow user type new lines
	else {
		cin.ignore(256, '\n');
		system("CLS");
		Editor si;
	}
}