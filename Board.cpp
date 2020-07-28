#include "Board.h"



//Prints board for debugging
void Board::print() {
	for(int y = 0; y < size.y; y++) {
		for(int x = 0; x < size.x; x++) {
			std::cout << board[y][x];
			if(x != size.x - 1) std::cout << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

//Prints board_states from debugging
void Board::print_states() {
	for(int y = 0; y < size.y; y++) {
		for(int x = 0; x < size.x; x++) {
			std::cout << board_states[y][x];
			if(x != size.x - 1) std::cout << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}



//Return bombs left
std::string Board::get_bombs_left() {
	return std::to_string(bombs_left);
}

//Return tile
int Board::get_tile(int x, int y) {
	return board[y][x];
}

//Return tile state
int Board::get_state(int x, int y) {
	return board_states[y][x];
}



//Clears board
void Board::reset() {
	bombs_left = bomb_count;

	//Set all positions to 0
	for(int y = 0; y < size.y; y++) {
		for(int x = 0; x < size.x; x++) {
			board[y][x] = 0;
			board_states[y][x] = 0;
		}
	}
}

//Sets up board with bombs and adjacent counts
void Board::populate(int cursor_x, int cursor_y) {
	int x, y;

	//Place bombs
	while(bombs_left > 0) {
		x = rand() % size.x;
		y = rand() % size.y;

		//If clear, place bomb
		if(board[y][x] < 9 && !((x <= cursor_x + 1 && x >= cursor_x - 1 ) && (y <= cursor_y + 1 && y >= cursor_y - 1 ))) {
			board[y][x] = 9;
			bombs_left--;

			//Change adjacent counts
			if(y - 1 >= 0 &&					   board[y - 1][x] < 9)		board[y - 1][x]++;
			if(y - 1 >= 0 &&	x + 1 < size.x &&  board[y - 1][x + 1] < 9) board[y - 1][x + 1]++;
			if(x + 1 < size.x &&				   board[y][x + 1] < 9)		board[y][x + 1]++;
			if(y + 1 < size.y && x + 1 < size.x && board[y + 1][x + 1] < 9) board[y + 1][x + 1]++;
			if(y + 1 < size.y &&				   board[y + 1][x] < 9)		board[y + 1][x]++;
			if(y + 1 < size.y && x - 1 >= 0 &&	   board[y + 1][x - 1] < 9) board[y + 1][x - 1]++;
			if(x - 1 >= 0 &&					   board[y][x - 1] < 9)		board[y][x - 1]++;
			if(y - 1 >= 0 &&	x - 1 >= 0 &&	   board[y - 1][x - 1] < 9) board[y - 1][x - 1]++;
		}
	}

	bombs_left = bomb_count;
}

//Reveal tile, recursive call for adjacent tiles if current tile has no adjacent bombs
bool Board::reveal(int x, int y) {
	if(board_states[y][x] == 0) {
		board_states[y][x] = 1;
		
		if(board[y][x] == 9) {
			board[y][x] = 10;
			return true;
		} else if(board[y][x] == 0) {
			if(y - 1 >= 0)						 reveal(x, y - 1);
			if(y - 1 >= 0 &&	x + 1 < size.x)	 reveal(x + 1, y - 1);
			if(x + 1 < size.x)					 reveal(x + 1, y);
			if(y + 1 < size.y && x + 1 < size.x) reveal(x + 1, y + 1);
			if(y + 1 < size.y)					 reveal(x, y + 1);
			if(y + 1 < size.y && x - 1 >= 0)	 reveal(x - 1, y + 1);
			if(x - 1 >= 0)						 reveal(x - 1, y);
			if(y - 1 >= 0 && x - 1 >= 0)		 reveal(x - 1, y - 1);
		}
	}
	return false;
}

//Handle flagging a tile
void Board::flag(int x, int y) {
	if(board_states[y][x] == 0) {
		board_states[y][x] = 2;
		bombs_left--;
	} else if(board_states[y][x] == 2) {
		board_states[y][x] = 0;
		bombs_left++;
	}
}

//Check to see if all non-bomb tiles are cleared
bool Board::check_squares() {
	for(int y = 0; y < size.y; y++) {
		for(int x = 0; x < size.x; x++) {
			if(board[y][x] < 9 && board_states[y][x] != 1) {
				return false;	
			}
		}
	}

	return true;
}

//Reveals remaining bombs after one is tripped
void Board::reveal_bombs() {
	for(int y = 0; y < size.y; y++) {
		for(int x = 0; x < size.x; x++) {
			if(board[y][x] == 9 && board_states[y][x] == 0) {
				board_states[y][x] = 1;
			} else if (board_states[y][x] == 2 && board[y][x] < 9) {
				board_states[y][x] = 3;
			}
		}
	}
}

//Flags remaining bombs after all other tiles are cleared
void Board::flag_bombs() {
	for(int y = 0; y < size.y; y++) {
		for(int x = 0; x < size.x; x++) {
			if(board[y][x] == 9 && board_states[y][x] == 0) {
				board_states[y][x] = 2;
			}
		}
	}

	bombs_left = 0;
}



//Constructs the Board object
Board::Board(int x, int y, int count) {
	size.x = x;
	size.y = y;
	bomb_count = count;

	board = new int *[size.y];
	board_states = new int *[size.y];
	for(int i = 0; i < size.y; i++) board[i] = new int[size.x];
	for(int i = 0; i < size.y; i++) board_states[i] = new int[size.x];

	reset();
}

