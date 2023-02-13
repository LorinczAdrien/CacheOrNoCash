#ifndef ROPE_GAME
#define ROPE_GAME

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "text_box.h"
#include "player.h"
#include "cpu_data.h"

using namespace std;

class CacheOrNoCash
{
	// Grafikai adatok
	sf::VideoMode video_mode;							// tulajdonkeppen csak a rezoluciot tarolja (pl: 1200x600)
	sf::RenderWindow* window;							// pointer egy 'renderelheto' ablakra, amelynek dinamikusan foglalunk memoriat

	// Background
	sf::Texture background_texture;
	sf::Sprite background_sprite;
	sf::Texture logo_texture;
	sf::Sprite logo_sprite;
	sf::Texture title_texture;
	sf::Sprite title_sprite;

	sf::RectangleShape CPU_element;
	sf::RectangleShape RAM_left, RAM_right;
	sf::RectangleShape Top;
	sf::RectangleShape Start;

	// Game elements
	// CPU processor;
	bool P1_done, P2_done, P3_done;
	sf::RectangleShape P1, P2, P3;
	sf::Text P1_text, P2_text, P3_text;
	sf::RectangleShape L1, L2, L3;
	sf::Text L1_text, L2_text, L3_text;

	// Blinkers (jelzik mennyire vannak tele a cache-k)
	sf::RectangleShape L1_blinker, L2_blinker, L3_blinker;

	// Events
	sf::Event event;									// kulonfele inputok feldolgozasara/azonositasara hasznaljuk

	// Fonts
	sf::Font font;										// egy adott fontban jelenik meg a text

	// Texts
	sf::Text timer;
	sf::Text points;
	sf::Text points_score;
	int gained_timer;
	sf::Text points_gained;

	// Mouse
	sf::Mouse mouse;
	sf::Vector2i mouse_poz_window;
	sf::Vector2f mouse_poz_float;

	// Input adatok
	TextBox box;

	// Game logic
	Player player;
	string player_name;
	int player_points;
	bool game_over;
	int time_left;										// megadja, hogy a jatek hany masodpercig fog tartani (masodpercek = frames/60)
	int current_time;									// eddig eltelt ido masodpercben

	bool title_screen;

	// Initializers
	void init_window();
	void init_variables();
	void init_font();
	void init_text();
	void init_texture();
	void init_shapes();

	// Other
	void poll_events();

	// CPU
	CPU_DATA processor;

public:

	class FileNotFound {};

	// Constructors and destructors
	CacheOrNoCash(string file_in, bool create_typos = false);

	// User functions
	void title_screen_loop();

	// Update
	void update();
	void update_clock();
	void update_mouse_position();
	void update_player_poz();
	void update_capacity_text();
	void update_spawn();
	void update_blinker(int ind);
	void update_points();
	
	// Interract
	void put_down_package();
	void pickup_package();

	// Render
	void render();
	void render_gui_text(sf::RenderTarget& target);
	void draw_textures(sf::RenderTarget& target);
	void draw_shapes(sf::RenderTarget& target);
	void draw_packages(sf::RenderTarget& target);

	// Getters
	bool is_running() const { return (this->window->isOpen() && !this->game_over); }
	bool is_on_title() { return this->title_screen; }
	sf::RenderWindow* get_window() { return this->window; }
	int get_points() { return this->player_points; }
	string get_name() { return this->player_name; }
};

#endif
