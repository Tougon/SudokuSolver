// Sudoku Solver
// This program takes a text file in a specific format for a 9x9 Sudoku Board, then gives the solution.
// 12/4/17
// @author Evan McGrath

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h> 

using namespace std;

// Class that represents a sudoku board. Inherits from vector
class SudokuBoard : public vector<vector<int>> {

public:
	// Board size represents the size of the board, square size represents the size of a square
	unsigned int const board_size, square_size;

	// Vector that contains all the possible answers. This represents a blank slot
	vector<int> empty_square = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	// Constructor builds a vector that contains all slots, and initializes the above values
	SudokuBoard(unsigned int s) : vector(s * s), board_size{ s }, square_size{ (unsigned int)sqrt(s) } {}

	void Solve();
	bool isCellEmpty(int h, int w) const;

private:
	bool isSolved();
	void crossVertical(int offsetW, int value);
	void crossHorizontal(int offsetH, int value);
	void crossBox(int offsetH, int offsetW, int value);

};


// Removes the given number from the possible answers in the column the value is present in.
// OffsetW indicates the proper column
void SudokuBoard::crossVertical(int offsetW, int value) {

	for (int i = offsetW; i < size(); i += board_size) {

		// Don't delete the value if it is already set, or another value is present
		if (at(i).size() > 1) {
			at(i).erase((std::remove(at(i).begin(), at(i).end(), value)), at(i).end());
		}

	}

}


// Removes the given number from the possible answers in the row the value is present in.
// OffsetH indicates the proper row
void SudokuBoard::crossHorizontal(int offsetH, int value) {

	for (int i = (offsetH * board_size); i < (board_size * (offsetH + 1)); i++) {

		// Don't delete the value if it is already set, or another value is present
		if (at(i).size() > 1) {
			at(i).erase((std::remove(at(i).begin(), at(i).end(), value)), at(i).end());
		}

	}

}


// Removes the given number from the possible answers in the box the value is present in.
// OffsetH and OffsetW indicate the specific box
void SudokuBoard::crossBox(int offsetH, int offsetW, int value) {

	// Binds the height to the first value in a box
	int height = (offsetH / 3) * 3;

	// Binds the height to the first value in a box
	int width = (offsetW / 3) * 3;

	for (int y = height; y < height + 3; y++) {

		for (int x = width; x < width + 3; x++) {

			// Don't delete the value if it is already set, or another value is present
			if (isCellEmpty(y, x)) {

				// Convert the value to a linear value so that the proper vector is cleared
				int i = (y * board_size) + x;
				at(i).erase((std::remove(at(i).begin(), at(i).end(), value)), at(i).end());
			}

		}

	}

}


// Indicates whether or not the cell is empty, or has more than one possible answer.
bool SudokuBoard::isCellEmpty(int h, int w) const
{
	return (at((h * board_size) + w).size() > 1);
}


// Indicates whether or not the puzzle is solved.
bool SudokuBoard::isSolved() {

	// Loops through the board, and if any cells contain more than one answer, returns false
	for (int y = 0; y < board_size; y++) {

		for (int x = 0; x < board_size; x++) {
			if (isCellEmpty(y, x)) {
				return false;
			}
		}

	}

	return true;
}


// Attempts to solve the board.
void SudokuBoard::Solve() {

	// Always checks once
	do {

		for (int y = 0; y < board_size; y++) {

			for (int x = 0; x < board_size; x++) {

				// If the cell has a solution
				if (!isCellEmpty(y, x)) {

					// Remove that digit from the possible answers in the same column
					crossVertical(x, at((y*board_size) + x)[0]);

					// Remove that digit from the possible answers in the same row
					crossHorizontal(y, at((y*board_size) + x)[0]);

					// Remove that digit from the possible answers in the same box
					crossBox(y, x, at((y*board_size) + x)[0]);
				}

			}

		}

		// Loops while the puzzle is not solved
	} while (!isSolved());

}


// Reads a Sudoku Board from a .puzzle file
istream& operator >> (istream& source, SudokuBoard& target) {

	string line;
	int currentRow = -1;

	while (getline(source, line)) {

		// Remove the lines used for formatting
		line.erase(std::remove(line.begin(), line.end(), '|'), line.end());
		line.erase(std::remove(line.begin(), line.end(), '-'), line.end());
		line.erase(std::remove(line.begin(), line.end(), '+'), line.end());

		// If the line contains any data, increment the current row
		if (line.size() > 0) {
			currentRow++;
		}

		// Look at each character in the line
		for (int i = 0; i < line.size(); i++) {

			// If the character exists, set the value in that row to the given value
			if (line[i] != ' ') {
				(target.at((currentRow * target.board_size) + i)) = vector<int>(1,((line[i] - '0')));
			}
			// Otherwise, assign an empty square
			else {
				(target.at((currentRow * target.board_size) + i)) = target.empty_square;
			}

		}

	}

	return source;
}


// Outputs a Sudoku Board to the file stream
ostream &operator<<(ostream& source, const SudokuBoard& target) {
	
	// Loop through the entire board

	for (int y = 0; y < target.board_size; y++) {

		for (int x = 0; x < target.board_size; x++) {

			// If the cell is not empty, output the value stored.
			if (!target.isCellEmpty(y, x)) {
				source << target.at((y * target.board_size) + x)[0];
			}
			// Otherwise, print a blank character
			else {
				source << " ";
			}

			// Print a vertical line every three characters, except after the last character
			if ((x + 1) % 3 == 0 && (x + 1) < 9) {
				source << "|";
			}

		}

		source << endl;

		// Print a horizontal line every three lines, except on the last line
		if ((y + 1) % 3 == 0 && (y + 1) < 9) {
			source << "---+---+---" << endl;
		}

	}

	return source;
}



int main(int argc, const char * argv[]) {

	SudokuBoard s{ 9 };

	string filename;

	cout << "Enter filename for puzzle file (include file extension):" << endl;
	cin >> filename;

	ifstream instream;
	instream.open(filename);

	if (!instream.good()) cout << "ERROR: File not found." << endl;

	else {
		instream >> noskipws;
		instream >> s;
	}

	cout << "\nUnsolved Board:\n" << s << endl;

	s.Solve();

	cout << "Solved Board:\n" << s << endl;


	instream.close();
	system("Pause");

	return 0;

}
