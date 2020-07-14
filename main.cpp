#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

#include <SFML/Graphics.hpp>



//Constants
const int border = 5;
const int tile_size = 16;
const int offset_y = border * 2 + tile_size;
const int scale = 1;



//HELPER FUNCTIONS
//Clamp integer between to integers, low is inclusive
int clamp(int i, int low, int high) {
	if(i < low) {
		i = low;
	} else if(i >= high) {
		i = high - 1;
	}

	return i;
}



//GAME FUNCTIONS
//Clears board
void reset_board(int **board, int size_x, int size_y) {
	//Set all positions to 0
	for(int y = 0; y < size_y; y++) {
		for(int x = 0; x < size_x; x++) {
			board[y][x] = 0;
		}
	}
}

//Sets up board with bombs and adjacent counts
void populate_board(int **board, int size_x, int size_y, int bombs_left, int cursor_x, int cursor_y) {
	int x, y;
	
	//Place bombs
	while(bombs_left > 0) {
		x = rand() % size_x;
		y = rand() % size_y;

		//If clear, place bomb
		if(board[y][x] < 9 && !((x <= cursor_x + 1 && x >= cursor_x - 1 ) && (y <= cursor_y + 1 && y >= cursor_y - 1 ))) {
			board[y][x] = 9;
			bombs_left--;

			//Change adjacent counts
			if(y - 1 >= 0 &&						board[y - 1][x] < 9)		board[y - 1][x]++;
			if(y - 1 >= 0 &&	x + 1 < size_x &&	board[y - 1][x + 1] < 9)	board[y - 1][x + 1]++;
			if(x + 1 < size_x &&					board[y][x + 1] < 9)		board[y][x + 1]++;
			if(y + 1 < size_y && x + 1 < size_x &&	board[y + 1][x + 1] < 9)	board[y + 1][x + 1]++;
			if(y + 1 < size_y &&					board[y + 1][x] < 9)		board[y + 1][x]++;
			if(y + 1 < size_y && x - 1 >= 0 &&		board[y + 1][x - 1] < 9)	board[y + 1][x - 1]++;
			if(x - 1 >= 0 &&						board[y][x - 1] < 9)		board[y][x - 1]++;
			if(y - 1 >= 0 &&	x - 1 >= 0 &&		board[y - 1][x - 1] < 9)	board[y - 1][x - 1]++;
		}
	}
} 

//Print board for debugging
/*void print_board(int **board, int size_x, int size_y) {
	for(int y = 0; y < size_y; y++) {
		for(int x = 0; x < size_x; x++) {
			std::cout << board[y][x];
			if(x != size_x - 1) std::cout << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}*/

//Reveal tile, recursive call for adjacent tiles if current tile has not adjacent bombs
bool reveal(int **board, int **board_states, int x, int y, int size_x, int size_y) {
	if(board_states[y][x] == 0) {
		board_states[y][x] = 1;
		
		if(board[y][x] == 9) {
			board[y][x] = 10;
			return true;
		} else if(board[y][x] == 0) {
			if(y - 1 >= 0)							reveal(board, board_states, x, y - 1, size_x, size_y);
			if(y - 1 >= 0 &&	x + 1 < size_x)		reveal(board, board_states, x + 1, y - 1, size_x, size_y);
			if(x + 1 < size_x)						reveal(board, board_states, x + 1, y, size_x, size_y);
			if(y + 1 < size_y && x + 1 < size_x)	reveal(board, board_states, x + 1, y + 1, size_x, size_y);
			if(y + 1 < size_y)						reveal(board, board_states, x, y + 1, size_x, size_y);
			if(y + 1 < size_y && x - 1 >= 0)		reveal(board, board_states, x - 1, y + 1, size_x, size_y);
			if(x - 1 >= 0)							reveal(board, board_states, x - 1, y, size_x, size_y);
			if(y - 1 >= 0 && x - 1 >= 0)			reveal(board, board_states, x - 1, y - 1, size_x, size_y);
		}
	}
	return false;
}

//Check to see if all non-bomb tiles are cleared
bool check_squares(int **board, int **board_states, int size_x, int size_y) {
	for(int y = 0; y < size_y; y++) {
		for(int x = 0; x < size_x; x++) {
			if(board[y][x] < 9 && board_states[y][x] != 1) {
				return false;	
			}
		}
	}

	return true;
}

//Reveals remaining bombs after one is tripped
void reveal_bombs(int **board, int **board_states, int size_x, int size_y) {
	for(int y = 0; y < size_y; y++) {
		for(int x = 0; x < size_x; x++) {
			if(board[y][x] == 9 && board_states[y][x] == 0) {
				board_states[y][x] = 1;
			} else if (board_states[y][x] == 2 && board[y][x] < 9) {
				board_states[y][x] = 3;
			}
		}
	}
}

//Flags remaining bombs after all other tiles are cleared
void flag_bombs(int **board, int **board_states, int size_x, int size_y) {
	for(int y = 0; y < size_y; y++) {
		for(int x = 0; x < size_x; x++) {
			if(board[y][x] == 9 && board_states[y][x] == 0) {
				board_states[y][x] = 2;
			}
		}
	}
}



//DRAWING FUNCTIONS
//Draws images
void draw_sprite(sf::RenderWindow &window, sf::Sprite tile, int x, int y, int scale) {
	tile.setScale(sf::Vector2f(scale, scale));
	tile.setPosition(sf::Vector2f(x * scale, y * scale));
	window.draw(tile);
}

//Draws text, x and y is top left
void draw_text(sf::RenderWindow &window, sf::Text &text, const std::string &s, int x, int y, int scale) {
	text.setString(s);
	text.setPosition(sf::Vector2f(x * scale, y * scale));
	window.draw(text);
}

//Draws text, x and y is top right
void draw_text_from_right(sf::RenderWindow &window, sf::Text &text, const std::string &s, int x, int y, int scale) {
	text.setString(s);
	sf::FloatRect bounds = text.getGlobalBounds();
	int width = bounds.width;
	text.setPosition(sf::Vector2f(x * scale - width, y * scale));
	window.draw(text);
}

//Draws cursor around selected tile
void draw_cursor(sf::RenderWindow &window, int cursor_x, int cursor_y, int size_x, int scale) {
	sf::RectangleShape cursor(sf::Vector2f(tile_size * scale, tile_size * scale));
	cursor.setOutlineThickness(2 * scale);
	cursor.setOutlineColor(sf::Color(255, 125, 0));
	cursor.setFillColor(sf::Color(0, 0, 0, 0));

	if(cursor_y == -1) {
		cursor.setPosition(sf::Vector2f((border + (size_x - 1) * tile_size / 2.0) * scale, border * scale));
	} else {
		cursor.setPosition(sf::Vector2f((border + cursor_x * tile_size) * scale, (offset_y + cursor_y * tile_size) * scale));
	}

	window.draw(cursor);
}



//Main Function
int main() {
	//Setup game parameters
	int size_x = 16, size_y = 8;
	int bomb_count = 20;
	int bombs_left = bomb_count;
	const int scale = 2;

	//State vars
	bool started = false;
	bool ended = false;
	bool won = false;
	
	//Other
	int cursor_x = 0, cursor_y = 0;

	//Setup timer
	sf::Clock clock;
	std::string time_ended = std::to_string((int)clock.getElapsedTime().asSeconds());

	//Setup random seed
	srand(time(NULL));



	//Create boards
	int **board;
	int **board_states;
	
	board = new int *[size_y];
	board_states = new int *[size_y];

	for(int i = 0; i < size_y; i++) board[i] = new int[size_x];
	for(int i = 0; i < size_y; i++) board_states[i] = new int[size_x];

	reset_board(board, size_x, size_y);
	reset_board(board_states, size_x, size_y);



	//Setup font and text
	sf::Font font;
	if(!font.loadFromFile("assets/ConsolaMono.ttf")) return 1;

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(tile_size * scale);



	//Load images
	sf::Texture tile_textures[17];
	sf::Sprite tiles[17];

	for(int i = 0; i < 9; i++) {
		if(!tile_textures[i].loadFromFile("assets/tile_" + std::to_string(i) + ".png")) return 1;
		tiles[i].setTexture(tile_textures[i]);
	}

	if(!tile_textures[9].loadFromFile("assets/tile_bomb.png")) return 1;
	tiles[9].setTexture(tile_textures[9]);
	if(!tile_textures[10].loadFromFile("assets/tile_boom.png")) return 1;
	tiles[10].setTexture(tile_textures[10]);
	if(!tile_textures[11].loadFromFile("assets/tile_blank.png")) return 1;
	tiles[11].setTexture(tile_textures[11]);
	if(!tile_textures[12].loadFromFile("assets/tile_flagged.png")) return 1;
	tiles[12].setTexture(tile_textures[12]);
	if(!tile_textures[13].loadFromFile("assets/tile_false_flagged.png")) return 1;
	tiles[13].setTexture(tile_textures[13]);
	if(!tile_textures[14].loadFromFile("assets/ok.png")) return 1;
	tiles[14].setTexture(tile_textures[14]);
	if(!tile_textures[15].loadFromFile("assets/cool.png")) return 1;
	tiles[15].setTexture(tile_textures[15]);
	if(!tile_textures[16].loadFromFile("assets/uncool.png")) return 1;
	tiles[16].setTexture(tile_textures[16]);



	//Create window
	sf::RenderWindow window(sf::VideoMode((size_x * tile_size + border * 2) * scale, ((size_y + 1) * tile_size + border * 3) * scale), "Minesweeper");



	//Main loop
	bool wait = false;

	while(window.isOpen()) {
		
		sf::Event event;
		while(window.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				window.close();
				break;
			} if(event.type == sf::Event::KeyPressed) {
                //Handle movement keys
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					cursor_y--;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					cursor_y++;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					cursor_x--;
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					cursor_x++;
				}
                
                //Clamp cursor and handle selecting face
				cursor_x = clamp(cursor_x, 0, size_x);
				cursor_y = clamp(cursor_y, -1, size_y);
				if(cursor_y == -1) {
					cursor_x = size_x / 2 - 1;
				}


                //Handle revealing and resetting
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
					if(cursor_y != -1 && !started) {
						populate_board(board, size_x, size_y, bomb_count, cursor_x, cursor_y);
						clock.restart();

						started = true;
					}
					
					if(cursor_y == -1 && started) {
						reset_board(board, size_x, size_y);
						reset_board(board_states, size_x, size_y);
						started = false;
						ended = false;
						won = false;
						bombs_left = bomb_count;
					} else if(!ended && started) {
						if(reveal(board, board_states, cursor_x, cursor_y, size_x, size_y)) {
							reveal_bombs(board, board_states, size_x, size_y);
							ended = true;
							
							std::cout << "Ended, bomb tripped." << std::endl;
						} else if(check_squares(board, board_states, size_x, size_y)) {
							flag_bombs(board, board_states, size_x, size_y);
							won = true;
							ended = true;
							
							std::cout << "Ended, all bombs cleared." << std::endl;
						}
						time_ended = std::to_string((int)clock.getElapsedTime().asSeconds());
					}
                //Handle flagging
				} else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
					if(!ended) {
						if(board_states[cursor_y][cursor_x] == 0) {
							board_states[cursor_y][cursor_x] = 2;
							bombs_left--;
						} else if(board_states[cursor_y][cursor_x] == 2) {
							board_states[cursor_y][cursor_x] = 0;
							bombs_left++;
						}
					}
				}

				wait = false;
			}
		}



        //Update screen whenever the clock increments
        if(started && !ended) {
            if(time_ended != std::to_string((int)clock.getElapsedTime().asSeconds())) {
                wait = false;
            }
        }



        //Draw frame
		if(!wait) {
			int tile_x, tile_y;

		    window.clear();

            //Draw tiles
			for(int y = 0; y < size_y; y++) {
				for(int x = 0; x < size_x; x++) {
					tile_x = border + x * tile_size;
					tile_y = offset_y + y * tile_size;

					if(board_states[y][x] == 0) {
						draw_sprite(window, tiles[11], tile_x, tile_y, scale);
					} else if(board_states[y][x] == 2) {
						draw_sprite(window, tiles[12], tile_x, tile_y, scale);
					} else if(board_states[y][x] == 3) {
						draw_sprite(window, tiles[13], tile_x, tile_y, scale);
					} else {
						draw_sprite(window, tiles[board[y][x]], tile_x, tile_y, scale);
					}
				}
			}

            //Draw face
			int face_x = border + (size_x - 1) * tile_size / 2;

			if(!ended) {
				draw_sprite(window, tiles[14], face_x, border, scale);
			} else if(won) {
				draw_sprite(window, tiles[15], face_x, border, scale);
			} else {
				draw_sprite(window, tiles[16], face_x, border, scale);
			}

            //Draw bombs left and timer
			draw_text(window, text, std::to_string(bombs_left), border, border, scale);
			if(!started) {
				draw_text_from_right(window, text, "0", border + size_x * tile_size, border, scale);
			} else if(!ended) {
				draw_text_from_right(window, text, std::to_string((int)clock.getElapsedTime().asSeconds()), border + size_x * tile_size, border, scale);
			} else {
				draw_text_from_right(window, text, time_ended, border + size_x * tile_size, border, scale);
			}
			
            //Draw cursor
			draw_cursor(window, cursor_x, cursor_y, size_x, scale);
			
            //Display frame
			window.display();
			
			wait = true;
		}
	}

	return 0;
}
