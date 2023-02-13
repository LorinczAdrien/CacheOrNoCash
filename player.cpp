#include "player.h"

// Initializers
void Player::init_variables()
{
	this->is_holding = false;
	this->movement_speed = 7.f;
}

void Player::init_shape()
{
	this->shape.setFillColor(sf::Color::Black);
	this->shape.setSize(sf::Vector2f(50.f, 50.f));
	// this->shape.setOutlineThickness(5);
	// this->shape.setOutlineColor(sf::Color::Color(72, 72, 72, 255));
}

// Constructors and destructors
Player::Player(float x, float y)
{
	this->holding_ind = 0;

	// Shape
	this->shape.setPosition(x, y);				// pozicio
	this->poz = { x,y };

	// Other init
	this->init_variables();
	this->init_shape();
}

Player::~Player()
{

}

// User functions
void Player::update(const sf::RenderTarget& target)
{
	// Window bounds collisions

	this->update_window_bounds_collision(target);
	this->update_input();
}

void Player::update_window_bounds_collision(const sf::RenderTarget& target)
{
	sf::FloatRect player_bounds = this->shape.getGlobalBounds();

	// Left
	if (player_bounds.left <= 0.f)
		this->update_poz({ 0.f, player_bounds.top });
	// Right
	else if (player_bounds.left + player_bounds.width >= target.getSize().x)
		this->update_poz({ target.getSize().x - player_bounds.width, player_bounds.top });

	player_bounds = this->shape.getGlobalBounds();

	// Top
	if (player_bounds.top <= 0.f)
		this->update_poz({ player_bounds.left, 0.f });
	// Bottom
	else if (player_bounds.top + player_bounds.height >= target.getSize().y)
		this->update_poz({ player_bounds.left, target.getSize().y - player_bounds.height });
}

void Player::update_input()
{
	// Keyboard input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->shape.move(sf::Vector2f(-this->movement_speed, 0.f));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->shape.move(sf::Vector2f(this->movement_speed, 0.f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->shape.move(sf::Vector2f(0.f, -this->movement_speed));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->shape.move(sf::Vector2f(0.f, this->movement_speed));
	}
}

void Player::update_poz(const sf::Vector2f& poz)
{
	this->poz = poz;
	this->shape.setPosition(poz);
}

void Player::update_poz_no_move(const sf::Vector2f& poz)
{
	this->poz = poz;
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}

const sf::FloatRect& Player::get_bounds() const
{
	return this->shape.getGlobalBounds();
}
