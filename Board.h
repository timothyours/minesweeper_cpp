#include <iostream>
#include <string>

struct Size {
	int x, y;
};

class Board {
	private:
		int bomb_count;
		int bombs_left;
		int **board;
		int **board_states;

	public:
		Size size;
		
		void print();
		void print_states();

		std::string get_bombs_left();
		int get_tile(int x, int y);
		int get_state(int x, int y);

		void reset();
		void populate(int cursor_x, int cursor_y);
		bool reveal(int x, int y);
		void flag(int x, int y);
		bool check_squares();
		void reveal_bombs();
		void flag_bombs();
		
		Board(int x, int y, int count);
};
