#ifndef JATEKOS
#define JATEKOS

#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

using namespace std;

class Player
{
	// Player attributes
	sf::RectangleShape shape;
	sf::Vector2f poz;
	
	int holding_ind;
	bool is_holding;
	float movement_speed;

	// Initializers
	void init_variables();
	void init_shape();

public:

	// Constructors and Destructors
	Player(float x = 0.f, float y = 0.f);
	~Player();

	// User functions
	void update(const sf::RenderTarget& target);
	void update_window_bounds_collision(const sf::RenderTarget& target);
	void update_input();
	void update_poz(const sf::Vector2f& poz);
	void update_poz_no_move(const sf::Vector2f& poz);

	void render(sf::RenderTarget& target);

	// Getters
	const sf::FloatRect& get_bounds() const;
	sf::Vector2f get_poz() { return this->poz; }
	bool get_holding() { return this->is_holding; }
	int get_holding_ind() { return this->holding_ind; }

	// Setter
	void set_poz(const sf::Vector2f& poz) { this->shape.setPosition(poz); this->poz = poz; }
	void set_color(sf::Color color) { this->shape.setFillColor(color); }
	void set_holding(bool holding)
	{
		this->is_holding = holding;
		if (holding) this->set_color(sf::Color::Cyan);
		else this->set_color(sf::Color::Black);
	}
	void set_holding_ind(int ind) { this->holding_ind = ind; }
};

#endif

