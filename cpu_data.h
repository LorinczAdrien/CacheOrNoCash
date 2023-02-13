#ifndef CENTRAL_PROCESSING_UNIT
#define CENTRAL_PROCESSING_UNIT

#include <time.h>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "splay_fa.h"

struct Package
{
	// Game
	sf::CircleShape shape;

	// Adatok
	bool is_on_screen, used;
	int place;
	int size;
	int priority;
	vector<double> elements;

	Package(int size = 0, int priority = 0);
};

class CPU_DATA
{
	// Tarolas
	int L1_size, L1_max;
	unordered_map<int, double> L1_cache;
	int L2_size, L2_max;
	unordered_map<int, double> L2_cache;
	int L3_size;
	SplayFa<double> L3_cache;

public:

	// Jatekhoz
	vector<Package> packages;

	// Constructors
	CPU_DATA(string input_file, int L1_size, int L2_size);

	// User functions
	void process_package(int package_ind, int place_ind, int& player_points, string& gained);
	void create_new_package();

	// Getters
	int get_L1_max() { return this->L1_max; }
	int get_L2_max() { return this->L2_max; }
	int get_L1_current() { return this->L1_size; }
	int get_L2_current() { return this->L2_size; }
	int get_L3_current() { return this->L3_size; }
};

#endif // ! CENTRAL_PROCESSING_UNIT