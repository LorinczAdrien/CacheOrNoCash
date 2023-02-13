#include "cache_jatek.h"

// Initializers
void CacheOrNoCash::init_window()
{
	this->video_mode = sf::VideoMode(1200, 1000);
	this->window = new sf::RenderWindow(this->video_mode, "Rope jatek!", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
}

void CacheOrNoCash::init_variables()
{
	this->P1_done = false;
	this->P2_done = false;
	this->P3_done = false;
	this->title_screen = true;

	this->player_points = 0;
	this->game_over = false;
	this->current_time = 0;
	this->gained_timer = 0;
	this->time_left = 120 * 60;															// 60 masodperc, mindegyik masodperc 60 frame
}

void CacheOrNoCash::init_font()
{
	if (!this->font.loadFromFile("Fonts/times new roman.ttf")) throw FileNotFound();	// ha nem sikerult betolteni a fontot akkor kivetelt valtunk ki
}

void CacheOrNoCash::init_text()
{
	// Eloszor inicializaljuk a GUI text objektumait
	this->timer.setFont(this->font); this->timer.setFillColor(sf::Color::Black);
	this->timer.setCharacterSize(40); this->timer.setString("Time: ");
	this->points = this->points_score = this->points_gained = this->timer;
	this->timer.setPosition({15.f, 20.f});
	this->points.setString("Points:      "); this->points_score.setString("0"); 
	this->points.move(sf::Vector2f(this->window->getSize().x - this->points.getGlobalBounds().width - 20, 20.f));
	this->points_gained = this->points; 
	this->points_gained.setString("+ 0");
	this->points_gained.move(sf::Vector2f(90.f, 30.f));
	this->points_score.move(sf::Vector2f(this->window->getSize().x - this->points_score.getGlobalBounds().width - 50, 20.f));

	// L1, L2, L3 cache
	this->L1_text.setFont(this->font);  this->L1_text.setFillColor(sf::Color::Black); 
	this->L1_text.setCharacterSize(25);  this->L1_text.setString("C: 0 / 0");
	this->L3_text = this->L2_text = this->L1_text;

	// L1 max
	stringstream stream;
	stream << "C: 0 / " << this->processor.get_L1_max();
	this->L1_text.setString(stream.str());

	// L2 max
	stream.str("");
	stream.clear();
	stream << "C: 0 / " << this->processor.get_L2_max();
	this->L2_text.setString(stream.str());

	// L3 max
	this->L3_text.setString("C: 0/ INF");

	this->L1_text.setPosition({ 365.f, 780.f });
	this->L2_text.setPosition({ 610.f, 780.f });
	this->L3_text.setPosition({ 550.f, 890.f });

	// P1, P2, P3
	this->P1_text.setFont(this->font); this->P1_text.setFillColor(sf::Color::Black);
	this->P1_text.setCharacterSize(20); this->P1_text.setString("...");
	this->P3_text = this->P2_text = this->P1_text;

	this->P1_text.setPosition({ 405.f, 160.f });
	this->P2_text.setPosition({ 565.f, 160.f });
	this->P3_text.setPosition({ 735.f, 160.f });
}

void CacheOrNoCash::init_texture()
{
	if (!this->background_texture.loadFromFile("Images/game_background.png"))
	{
		cout << "Nem sikerult a hatter texture-t betolteni!\n";
	}

	this->background_sprite.setTexture(this->background_texture);

	if (!this->logo_texture.loadFromFile("Images/game_logo.png"))
	{
		cout << "Nem sikerult a hatter texture-t betolteni!\n";
	}

	this->logo_sprite.setTexture(this->logo_texture);
	this->logo_sprite.setPosition({ 280.f, 0.f });

	if (!this->title_texture.loadFromFile("Images/title_background.png"))
	{
		cout << "Nem sikerult a hatter texture-t betolteni!\n";
	}

	this->title_sprite.setTexture(this->title_texture);
}

void CacheOrNoCash::init_shapes()
{
	// CPU
	this->CPU_element.setPosition({ 391.f, 327.f });
	this->CPU_element.setSize({ 415.f, 415.f }); this->CPU_element.setFillColor(sf::Color::Black);

	// RAMs
	this->RAM_left.setPosition({ 0.f, 100.f });
	this->RAM_left.setSize({ 180.f, 900.f }); this->RAM_left.setFillColor(sf::Color::Black);

	this->RAM_right.setPosition({ 1025.f, 100.f });
	this->RAM_right.setSize({ 180.f, 900.f }); this->RAM_right.setFillColor(sf::Color::Black);

	// Top
	this->Top.setPosition({ 0.f, 0.f });
	this->Top.setSize({ 1200.f, 100.f }); this->Top.setFillColor(sf::Color::Black);

	// P1
	this->P1.setPosition({ 359.f, 165.f });
	this->P1.setSize({ 132.f, 102.f }); this->P1.setFillColor(sf::Color::Black);
	// P2
	this->P2.setPosition({ 525.f, 165.f });
	this->P2.setSize({ 132.f, 102.f }); this->P2.setFillColor(sf::Color::Black);
	// P2
	this->P3.setPosition({ 697.f, 165.f });
	this->P3.setSize({ 132.f, 102.f }); this->P3.setFillColor(sf::Color::Black);

	// L1 cache
	this->L1.setPosition({ 358.f, 758.f });
	this->L1.setSize({ 132.f, 102.f }); this->L1.setFillColor(sf::Color::Black);
	// L2 cache
	this->L2.setPosition({ 506.f, 758.f });
	this->L2.setSize({ 336.f, 102.f }); this->L2.setFillColor(sf::Color::Black);
	// L3 cache
	this->L3.setPosition({ 358.f, 865.f });
	this->L3.setSize({ 483.f, 113.f }); this->L3.setFillColor(sf::Color::Black);

	// Blinkers
	this->L1_blinker.setSize({ 20.f, 20.f }); this->L1_blinker.setFillColor(sf::Color::Green); 
	this->L1_blinker.setOutlineThickness(3); this->L1_blinker.setOutlineColor(sf::Color::Black);
	this->L3_blinker = this->L2_blinker = this->L1_blinker;

	this->L1_blinker.setPosition({ 416.f, 824.f });
	this->L2_blinker.setPosition({ 654.f, 824.f });
	this->L3_blinker.setPosition({ 590.f, 935.f });

	// Start button
	this->Start.setSize({ 320.f, 64.f }); this->Start.setFillColor(sf::Color::Black);
	this->Start.setPosition({ 440.f, 360.f });
}

void CacheOrNoCash::poll_events()
{
	while (this->window->pollEvent(this->event))
	{
		if (this->title_screen)
		{
			switch (this->event.type)
			{
			case sf::Event::Closed:																	// Ha a felhasznalo kerte, hogy zarjuk be az ablakot
				this->window->close();
				break;

			case sf::Event::KeyPressed:
				if (this->event.key.code == sf::Keyboard::Left || this->event.key.code == sf::Keyboard::Right) this->box.process_event(event);	// Ha mozditani szeretnenk a cursort jobbra vagy balra
				break;

			case sf::Event::MouseButtonPressed:
				if (this->mouse.isButtonPressed(sf::Mouse::Left))									// Megnezzuk ha eppen kivalasztottuk valamelyik text boxot
				{
					// Ha startra nyomott
					if (this->Start.getGlobalBounds().contains(this->mouse_poz_float)) this->title_screen = false;

					if (this->box.get_bounds().contains(this->mouse_poz_float)) this->box.set_selected(true, this->mouse_poz_float);
					else this->box.set_selected(false, this->mouse_poz_float);
				}
				break;

			case sf::Event::TextEntered:
				this->box.process_event(event);
				break;
			}
		}
		else
		{
			switch (this->event.type)
			{
			case sf::Event::Closed:																	// Ha a felhasznalo kerte, hogy zarjuk be az ablakot
				this->window->close();
				break;

			case sf::Event::KeyPressed:
				if (this->event.key.code == sf::Keyboard::Escape) this->window->close();			// ESC key eseten
				if (this->event.key.code == sf::Keyboard::E) {
					this->pickup_package();
					this->put_down_package();
				}
				break;
			}
		}
	}
}

// Constructors and destructors
CacheOrNoCash::CacheOrNoCash(string file_in, bool create_typos)
	: box(50, sf::Color::Color(175, 175, 175, 255), font, false, "Name: "),
	processor(file_in, 100, 300)
{
	ifstream fin(file_in);

	if (!fin.is_open()) throw FileNotFound();

	this->init_window();
	this->init_texture();
	this->init_variables();
	this->init_font();
	this->init_text();
	this->init_shapes();

	// Input box
	this->box.set_poz({ 390.f, 460.f });

	// Player name
	this->player_name = "";

	// Player coordinates (starting coordiantes)
	this->player.set_poz({ 560.f, 190.f });
}

void CacheOrNoCash::title_screen_loop()
{
	while (this->title_screen)
	{
		this->window->clear(sf::Color::White);

		// Mouse poz
		this->update_mouse_position();

		// Background
		this->window->draw(this->title_sprite);

		this->box.draw_to_target(*this->window);

		// Szukseges pollok
		this->poll_events();

		this->window->display();
	}

	// Jatekos neve
	this->player_name = this->box.get_text();
	this->player_name.erase(0, 5);
}

// User functions
void CacheOrNoCash::update()
{
	// Minden frame eseten megnezzuk, ha szukseges barmilyen inputot feldolgozni
	this->poll_events();

	// Update player
	this->update_player_poz();

	// Frissitjuk az idozitot
	this->update_clock();
	this->update_mouse_position();

	// Frissitjuk a cache capacity texteket es pontokat
	this->update_capacity_text();
	this->update_points();

	// Frissit spawnalt packagek
	this->update_spawn();

	// Frissit blinkers
	this->update_blinker(1);
	this->update_blinker(2);
}

void CacheOrNoCash::update_clock()
{
	if (this->time_left <= 0)
	{
		this->game_over = true;								// tehat vege a jateknak				
	}
	else
	{
		this->time_left--;
		this->current_time = this->time_left / 60;
		stringstream timer_stream;
		timer_stream << "Time: " << this->current_time;
		this->timer.setString(timer_stream.str());
	}
}

void CacheOrNoCash::update_mouse_position()
{
	// frissiti a mouse helyzetet (a kepernyohoz viszonyitva)
	this->mouse_poz_window = sf::Mouse::getPosition(*this->window);
	this->mouse_poz_float = this->window->mapPixelToCoords(this->mouse_poz_window);
	// cout << "Mouse: x = " << mouse_poz_float.x << ", y = " << mouse_poz_float.y << '\n';
}

void CacheOrNoCash::update_player_poz()
{
	// Eloszor mozdulni fog (es megnezi ha kimenne a kepernyorol)
	this->player.update(*this->window);

	// Most megnezzuk ha utkozik a CPU-val
	sf::FloatRect CPU_bounds = CPU_element.getGlobalBounds();
	sf::FloatRect player_bounds = this->player.get_bounds();

	// Update player jelenlegi poz
	this->player.update_poz_no_move({ player_bounds.left, player_bounds.top });
	sf::Vector2f player_poz = this->player.get_poz();
	
	if (player_bounds.intersects(CPU_bounds))
	{
		// Top eset
		if (player_bounds.top < CPU_bounds.top)
		{
			player_poz.y = CPU_bounds.top - player_bounds.height;			// kitesszuk a kockabol (csak y valtozik)
			this->player.set_poz(player_poz);
		}
		// Bottom eset
		else if (player_bounds.top > CPU_bounds.top + CPU_bounds.height - 10)
		{
			player_poz.y = CPU_bounds.top + CPU_bounds.height;				// kitesszuk a kockabol (csak y valtozik)
			this->player.set_poz(player_poz);
		}
		// Bal eset
		else if (player_bounds.left < CPU_bounds.left)
		{
			player_poz.x = CPU_bounds.left - player_bounds.width;			// kitesszuk a kockabol (csak x valtozik)
			this->player.set_poz(player_poz);
		}
		// Jobb eset
		else
		{
			player_poz.x = CPU_bounds.left + CPU_bounds.width;				// kitesszuk a kockabol (csak x valtozik)
			this->player.set_poz(player_poz);
		}
	}

	// Ha utkozik a 2 RAM stickel
	sf::FloatRect RAML_bounds = RAM_left.getGlobalBounds();
	if (player_bounds.intersects(RAML_bounds))
	{
		player_poz.x = 175;													// kitesszuk a Rambol (csak x valtozik)
		this->player.set_poz(player_poz);
	}

	sf::FloatRect RAMR_bounds = RAM_right.getGlobalBounds();
	if (player_bounds.intersects(RAMR_bounds))
	{
		player_poz.x = 1032 - player_bounds.width;							// kitesszuk a Rambol (csak x valtozik)
		this->player.set_poz(player_poz);
	}

	// Ha utkozik a fejleccel
	sf::FloatRect Top_bounds = Top.getGlobalBounds();
	if(player_bounds.intersects(Top_bounds))
	{
		player_poz.y = 100;													// Kitesszuk a top-bol (csak y valtozik)
		this->player.set_poz(player_poz);
	}
}

void CacheOrNoCash::update_capacity_text()
{
	stringstream stream;
	// L1 text
	stream << "C: " << this->processor.get_L1_current() << " / " << this->processor.get_L1_max();
	this->L1_text.setString(stream.str());

	stream.str(""); stream.clear();

	// L2 text
	stream << "C: " << this->processor.get_L2_current() << " / " << this->processor.get_L2_max();
	this->L2_text.setString(stream.str());

	stream.str(""); stream.clear();

	// L3
	stream << "C: " << this->processor.get_L3_current() << " / INF";
	this->L3_text.setString(stream.str());
}

void CacheOrNoCash::update_spawn()
{
	stringstream stream;
	int random = rand() % 100;											// 1/50 esely eseten spawn a package
	if (random == 0)
	{
		for (int i = 0; i < this->processor.packages.size(); ++i)		// kivalasztunk egyet amelyik meg nem volt a kepernyon
		{
			if (!this->processor.packages[i].is_on_screen && !this->processor.packages[i].used)
			{
				if (!this->P1_done)
				{
					this->P1_done = true;

					// Text
					stream << this->processor.packages[i].size << "/" << this->processor.packages[i].priority;
					this->P1_text.setString(stream.str());

					// Package
					this->processor.packages[i].shape.setPosition({ 395.f, 185.f });
					this->processor.packages[i].is_on_screen = true;
					this->processor.packages[i].used = true;
					this->processor.packages[i].place = 1;
				}
				else if (!this->P2_done)
				{
					this->P2_done = true;
					
					// Text
					stream << this->processor.packages[i].size << "/" << this->processor.packages[i].priority;
					this->P2_text.setString(stream.str());

					// Package
					this->processor.packages[i].shape.setPosition({ 570.f, 185.f });
					this->processor.packages[i].is_on_screen = true;
					this->processor.packages[i].used = true;
					this->processor.packages[i].place = 2;
				}
				else if (!this->P3_done)
				{
					this->P3_done = true;

					// Text
					stream << this->processor.packages[i].size << "/" << this->processor.packages[i].priority;
					this->P3_text.setString(stream.str());

					// Package
					this->processor.packages[i].shape.setPosition({ 740.f, 185.f });
					this->processor.packages[i].is_on_screen = true;
					this->processor.packages[i].used = true;
					this->processor.packages[i].place = 3;
				}
				break;
			}
		}
	}

	random = rand() % 200;
	if (random == 0)									// Ha a processzor megint hasznal egy csomagot akkor letrehoz egy ujjat (mas merettel es ertekkel)
	{
		this->processor.create_new_package();
	}
}

void CacheOrNoCash::pickup_package()
{
	if (!this->player.get_holding())							// Ha a jatekos meg nem fog semmit
	{
		sf::FloatRect player_bounds = this->player.get_bounds();
		for (int package = 0; package < this->processor.packages.size(); ++package)
		{
			if (this->processor.packages[package].is_on_screen)							// Ha a kepernyon van
			{
				sf::FloatRect package_bounds = this->processor.packages[package].shape.getGlobalBounds();
				if (player_bounds.intersects(package_bounds))
				{
					this->processor.packages[package].is_on_screen = false;
					this->player.set_holding(true);
					this->player.set_holding_ind(package);

					if (this->processor.packages[package].place == 1)
					{
						this->P1_done = false;
					}
					else if (this->processor.packages[package].place == 2)
					{
						this->P2_done = false;
					}
					else
					{
						this->P3_done = false;
					}
				}
			}
		}
	}
}

void CacheOrNoCash::put_down_package()
{
	string gained;
	if (this->player.get_holding())										// Ha a jatekos tart valamit csak akkor nezzuk meg ha tehet le
	{
		sf::FloatRect player_bounds = this->player.get_bounds();

		if (player_bounds.intersects(this->L1.getGlobalBounds()))
		{
			// Process package
			this->processor.process_package(this->player.get_holding_ind(), 4, this->player_points, gained);

			// Update player state
			this->player.set_holding(false);
			this->player.set_holding_ind(-1);

			// Update blinker
			this->update_blinker(1);
			this->gained_timer = 120;

			// Update gained points
			this->points_gained.setString(gained);
		}
		else if (player_bounds.intersects(this->L2.getGlobalBounds()))
		{
			// Process package
			this->processor.process_package(this->player.get_holding_ind(), 5, this->player_points, gained);

			// Update player state
			this->player.set_holding(false);
			this->player.set_holding_ind(-1);

			// Update blinker
			this->update_blinker(2);
			this->gained_timer = 120;

			// Update gained points
			this->points_gained.setString(gained);
		}
		else if (player_bounds.intersects(this->L3.getGlobalBounds()))
		{
			// Process package
			this->processor.process_package(this->player.get_holding_ind(), 6, this->player_points, gained);

			// Update player state
			this->player.set_holding(false);
			this->player.set_holding_ind(-1);
			this->gained_timer = 120;

			// Update gained points
			this->points_gained.setString(gained);
		}
	}
}

void CacheOrNoCash::update_blinker(int ind)
{
	// Szint valtoztatunk aszerint mennyire van tele egy adott cache
	if (ind == 1)
	{
		double percent_full = (static_cast<double>(this->processor.get_L1_current()) / static_cast<double>(this->processor.get_L1_max())) * 100;

		if (percent_full >= 25 && percent_full < 50)
		{
			this->L1_blinker.setFillColor(sf::Color::Yellow);
		}
		else if (percent_full >= 50 && percent_full < 75)
		{
			this->L1_blinker.setFillColor(sf::Color::Color(201, 64, 14, 255));
		}
		else if(percent_full >= 75)
		{
			this->L1_blinker.setFillColor(sf::Color::Red);
		}
	}
	else if (ind == 2)
	{
		double percent_full = (static_cast<double>(this->processor.get_L2_current()) / static_cast<double>(this->processor.get_L2_max())) * 100;

		if (percent_full >= 25 && percent_full < 50)
		{
			this->L2_blinker.setFillColor(sf::Color::Yellow);
		}
		else if (percent_full >= 50 && percent_full < 75)
		{
			this->L2_blinker.setFillColor(sf::Color::Color(201, 64, 14, 255));
		}
		else if (percent_full >= 75)
		{
			this->L2_blinker.setFillColor(sf::Color::Red);
		}
	}
}

void CacheOrNoCash::update_points()
{
	stringstream points;
	points << this->player_points;
	this->points_score.setString(points.str());
}

void CacheOrNoCash::render()
{
	// A volt frame-t 'toroljuk' (valojaban csak egy ures canvas-t keszitunk, amire helyezhetjuk az objektumainkat)
	this->window->clear();

	// VALTOZTATASOK RESZ
	this->draw_textures(*this->window);					// Background
	// this->box.draw_to_target(*this->window);			// Text

	// Update shapes (temp)
	this->draw_shapes(*this->window);

	// Player
	this->player.render(*this->window);

	// Packages
	this->draw_packages(*this->window);

	// A GUI reszeit is megjelenitjuk
	this->render_gui_text(*this->window);
	// VALTOZTATASOK RESZ

	// A feldolgozott frame-t mostmar renderelhetjuk
	this->window->display();
}

void CacheOrNoCash::render_gui_text(sf::RenderTarget& target)
{
	// Top
	target.draw(this->timer);
	target.draw(this->points);
	target.draw(this->points_score);
	if (this->gained_timer > 0)
	{
		target.draw(this->points_gained);
		this->gained_timer--;
	}

	// Counters
	target.draw(this->L1_text);
	target.draw(this->L2_text);
	target.draw(this->L3_text);

	// Package info
	if (P1_done) target.draw(this->P1_text);
	if (P2_done) target.draw(this->P2_text);
	if (P3_done) target.draw(this->P3_text);
}

void CacheOrNoCash::draw_textures(sf::RenderTarget& target)
{
	target.draw(this->background_sprite);
	target.draw(this->logo_sprite);
}

void CacheOrNoCash::draw_shapes(sf::RenderTarget& target)
{
	// target.draw(this->CPU_element);
	// target.draw(this->RAM_left);
	// target.draw(this->RAM_right);
	// target.draw(this->Top);
	// target.draw(this->L1); target.draw(this->L2); target.draw(this->L3);
	// target.draw(this->P1);
	// target.draw(this->P2);
	// target.draw(this->P3);

	// Blinkers
	target.draw(this->L1_blinker);
	target.draw(this->L2_blinker);
	target.draw(this->L3_blinker);
}

void CacheOrNoCash::draw_packages(sf::RenderTarget& target)
{
	for (int i = 0; i < this->processor.packages.size(); ++i)
	{
		if (this->processor.packages[i].is_on_screen)
		{
			target.draw(this->processor.packages[i].shape);
		}
	}
}
