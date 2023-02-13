#ifndef BOX_OF_TEXT
#define BOX_OF_TEXT

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace std;

#define DELETE_KEY 8
#define ENTER_KEY 13

class TextBox
{
	// TextBox elemei
	sf::Text text_box;										// A text box class implementaciojahoz egy sf::Text objektumot hasznalunk
	int jelen_ind;
	bool is_selected;

	// A text box-hoz tartozik egy cursor is amit valos idoben frissitunk
	sf::Text cursor;
	sf::Vector2f cursor_poz;

	// Utility
	ostringstream text;

	void process_character(int irt_karakter);
	void process_arrow_input(bool left);
	void delete_character(int ind);

public:

	TextBox(int karakter_meret, sf::Color szin, const sf::Font& font, bool currently_selected, string kezdeti_string);

	void draw_to_target(sf::RenderWindow& target);
	void process_event(const sf::Event& event);

	// Getterek
	string get_text() { return this->text.str(); }
	int text_size() { return this->text.str().length(); }
	bool text_empty() { return this->text.str().length() == 0; }
	sf::FloatRect get_bounds() { return this->text_box.getGlobalBounds(); }

	// Setterek
	void set_poz(const sf::Vector2f& poz);
	void set_cursor_poz(int string_ind);
	void set_selected(bool selected, const sf::Vector2f& mouse_poz);
};

#endif
