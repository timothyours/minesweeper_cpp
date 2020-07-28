#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "Board.h"

#include <SFML/Graphics.hpp>



//Constants
const int border = 5;
const int tile_size = 16;
const int offset_y = border * 2 + tile_size;



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



//INPUT FUNCTIONS
//Check if mouse is in tile field
bool mouse_in_tiles(sf::Vector2i mouse, int window_width, int window_height, int scale) {
	return mouse.x >= border * scale && mouse.x < window_width - border * scale &&
		   mouse.y >= offset_y * scale && mouse.y < window_height - border * scale;
}

//Check if the mouse is on the face
bool mouse_on_face(sf::Vector2i mouse, int window_width, int scale) {
	return mouse.x >= window_width / 2 - tile_size / 2 * scale &&
		   mouse.x < window_width / 2 + tile_size / 2 * scale &&
		   mouse.y >= border * scale && mouse.y < (border + tile_size) * scale;
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
int main(int argc, char **argv) {
	//Setup game parameters
	int sx, sy, bc, scale;

	if(argc < 3) {
		sx = 16;
		sy = 16;
	} else {
		sx = std::stoi(argv[1]);
		sy = std::stoi(argv[2]);
	}
	
	if(argc < 4) bc = (int)(sx * sy * 0.15625);
	else bc = std::stoi(argv[3]);


	if(argc < 5) scale = 2;
	else scale = std::stoi(argv[4]);

	//State vars
	bool started = false;
	bool ended = false;
	bool won = false;

	//Construct board
	Board board(sx, sy, bc);
	
	//Other
	int cursor_x = 0, cursor_y = 0, previous_cursor_x = 0, previous_cursor_y = 0;
	int window_width = (board.size.x * tile_size + border * 2) * scale;
	int window_height = ((board.size.y + 1) * tile_size + border * 3) * scale;
	sf::Vector2i previous_mouse;
	sf::Vector2i current_mouse;

	//Setup timer
	sf::Clock clock;
	std::string time_ended = std::to_string((int)clock.getElapsedTime().asSeconds());

	//Setup random seed
	srand(time(NULL));



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
	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper");
	previous_mouse = sf::Mouse::getPosition(window);



	//Main loop
	bool wait = false;

	while(window.isOpen()) {
		sf::Event event;

		while(window.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				window.close();
				break;
			}
			
            //Handle movement keys
			if(event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
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
				cursor_x = clamp(cursor_x, 0, board.size.x);
				cursor_y = clamp(cursor_y, -1, board.size.y);
				if(cursor_y == -1) {
					cursor_x = board.size.x / 2 - 1;
				}


                //Handle revealing and resetting
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
				   sf::Keyboard::isKeyPressed(sf::Keyboard::X) ||
				   sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if(cursor_y != -1 && !started &&
					   !(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
					   !mouse_in_tiles(current_mouse, window_width, window_height, scale))) {
						board.populate(cursor_x, cursor_y);
						clock.restart();

						started = true;
					}
					
					if(cursor_y == -1 && started &&
					   !(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
					   !mouse_on_face(current_mouse, window_width, scale))) {
						board.reset();
						started = false;
						ended = false;
						won = false;
					} else if(!ended && started &&
							  !(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
							  !mouse_in_tiles(current_mouse, window_width, window_height, scale))) {
						if(board.reveal(cursor_x, cursor_y)) {
							board.reveal_bombs();
							ended = true;
							
							std::cout << "Ended, bomb tripped." << std::endl;
						} else if(board.check_squares()) {
							board.flag_bombs();
							won = true;
							ended = true;
							
							std::cout << "Ended, all bombs cleared." << std::endl;
						}
						time_ended = std::to_string((int)clock.getElapsedTime().asSeconds());
					}
				} 
				
                //Handle flagging
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
						  sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ||
						  sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					if(!ended &&
					   !(sf::Mouse::isButtonPressed(sf::Mouse::Right) &&
					   !mouse_in_tiles(current_mouse, window_width, window_height, scale))) {
						board.flag(cursor_x, cursor_y);
					}
				}

				wait = false;
			}
			


			//Handle mouse movement
			if(event.type == sf::Event::MouseMoved) {
				current_mouse = sf::Mouse::getPosition(window);
		
				if(!(current_mouse.x == previous_mouse.x && current_mouse.y == previous_mouse.y)) {
					previous_cursor_x = cursor_x;
					previous_cursor_y = cursor_y;
			
					if(mouse_in_tiles(current_mouse, window_width, window_height, scale)) {
						cursor_x = (int)((current_mouse.x / scale - border) / tile_size);
						cursor_y = (int)((current_mouse.y / scale - offset_y) / tile_size);
					}

					if(mouse_on_face(current_mouse, window_width, scale)) {
						cursor_x = board.size.x / 2 - 1;
						cursor_y = -1;
					}
			
					if(!(cursor_x == previous_cursor_x && cursor_y == previous_cursor_y)) {
						wait = false;
					}
				}

				previous_mouse = current_mouse;
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
			for(int y = 0; y < board.size.y; y++) {
				for(int x = 0; x < board.size.x; x++) {
					tile_x = border + x * tile_size;
					tile_y = offset_y + y * tile_size;

					if(board.get_state(x, y) == 0) {
						draw_sprite(window, tiles[11], tile_x, tile_y, scale);
					} else if(board.get_state(x, y) == 2) {
						draw_sprite(window, tiles[12], tile_x, tile_y, scale);
					} else if(board.get_state(x, y) == 3) {
						draw_sprite(window, tiles[13], tile_x, tile_y, scale);
					} else {
						draw_sprite(window, tiles[board.get_tile(x, y)], tile_x, tile_y, scale);
					}
				}
			}

            //Draw face
			int face_x = border + (board.size.x - 1) * tile_size / 2;

			if(!ended) {
				draw_sprite(window, tiles[14], face_x, border, scale);
			} else if(won) {
				draw_sprite(window, tiles[15], face_x, border, scale);
			} else {
				draw_sprite(window, tiles[16], face_x, border, scale);
			}

            //Draw bombs left and timer
			draw_text(window, text, board.get_bombs_left(), border, border, scale);
			if(!started) {
				draw_text_from_right(window, text, "0", border + board.size.x * tile_size, border, scale);
			} else if(!ended) {
				draw_text_from_right(window, text, std::to_string((int)clock.getElapsedTime().asSeconds()), border + board.size.x * tile_size, border, scale);
			} else {
				draw_text_from_right(window, text, time_ended, border + board.size.x * tile_size, border, scale);
			}
			
            //Draw cursor
			draw_cursor(window, cursor_x, cursor_y, board.size.x, scale);
			
            //Display frame
			window.display();
			
			wait = true;
		}
	}

	return 0;
}
