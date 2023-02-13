#include "text_box.h"

// Utility
void TextBox::process_character(int irt_karakter)
{
	if (irt_karakter != DELETE_KEY && irt_karakter != ENTER_KEY)
	{
		this->jelen_ind++;
		string jelen_szoveg = text.str();

		jelen_szoveg.insert(this->jelen_ind, 1, static_cast<char>(irt_karakter));

		// Update stream
		this->text.str("");
		this->text.clear();
		this->text << jelen_szoveg;
	}
	else if (irt_karakter == DELETE_KEY)
	{
		if (!this->text_empty() && this->jelen_ind != -1)						// ha van amit torolni
		{
			this->delete_character(this->jelen_ind);
			this->jelen_ind--;
		}
	}
	// Frissitjuk a szoveget
	this->text_box.setString(text.str());										// a kapott string

	// Beallitjuk a cursor poziciojat a megfelelo helyre
	this->set_cursor_poz(this->jelen_ind + 1);
}

void TextBox::process_arrow_input(bool left)
{
	if (left)
	{
		if (this->jelen_ind >= 0)								// Ha mozoghat a cursot balra
		{
			this->set_cursor_poz(this->jelen_ind);
			this->jelen_ind--;
		}
	}
	else
	{
		if (this->jelen_ind < this->text_size() - 1)			// Ha mozoghat a cursor jobbra
		{
			this->jelen_ind++;
			this->set_cursor_poz(this->jelen_ind + 1);
		}
	}
}

void TextBox::delete_character(int ind)
{
	string jelen_szoveg = this->text.str();

	jelen_szoveg.erase(ind, 1);

	// Az uj karakterlancot visszahelyezzuk a text_box-ba
	this->text.str("");
	this->text << jelen_szoveg;
	this->text_box.setString(this->text.str());
}

void TextBox::set_poz(const sf::Vector2f& poz)
{
	// Update text_box poz
	this->text_box.setPosition(poz);

	// Update cursor poz (automatikusan a vegere megy)
	this->set_cursor_poz(text.str().length());
}

void TextBox::set_cursor_poz(int string_ind)
{
	// Ha nem helyes indexet adunk meg akkor alapertelmezetten a cursor a vegere lesz allitva
	this->cursor_poz = this->text_box.findCharacterPos(string_ind);
	this->cursor.setPosition(cursor_poz);
}

void TextBox::set_selected(bool selected, const sf::Vector2f& mouse_poz)
{
	// Beallitjuk az 'is_selected' valtozot
	this->is_selected = selected;

	if (this->is_selected)									// Ha ki van valasztva, akkor update mouse_poz
	{
		int min_tavolsag_ind = INT32_MAX;
		int jelen_tavolsag_mouse = 0, tavolsag_mouse = INT32_MAX;
		string jelen_szoveg = text.str();

		sf::Vector2f jelen_karakter_poz, uj_cursor_poz;
		for (int i = 0; i < jelen_szoveg.length(); ++i)
		{
			jelen_karakter_poz = this->text_box.findCharacterPos(i);

			if (abs(mouse_poz.x - jelen_karakter_poz.x) < tavolsag_mouse)
			{
				tavolsag_mouse = abs(mouse_poz.x - jelen_karakter_poz.x);
				uj_cursor_poz = jelen_karakter_poz;
				min_tavolsag_ind = i;
			}
		}

		// Lementjuk, hogy a cursor jelenleg melyik karaktert torolne, vagy melyik karakter utan irna (ha -1 tudjuk, hogy az elejen van)
		this->jelen_ind = min_tavolsag_ind - 1;

		// Update cursor poz
		this->cursor.setPosition(uj_cursor_poz);
	}
}

// Constructors and destructors
TextBox::TextBox(int karakter_meret, sf::Color szin, const sf::Font& font, bool currently_selected, string kezdeti_string)
{
	// A kapott stringet a folyamba helyezzuk
	this->text << kezdeti_string;

	this->jelen_ind = kezdeti_string.length() - 1;					// az utolso karakter utani helyre mutat

	// text_box general
	this->text_box.setFont(font);
	this->text_box.setCharacterSize(karakter_meret);
	this->text_box.setFillColor(szin);

	// TextBox adatok
	this->is_selected = currently_selected;

	// Cursor adatok
	this->cursor.setFont(font);
	this->cursor.setCharacterSize(karakter_meret);
	this->cursor.setFillColor(szin);
	this->cursor.setString("|");

	this->text_box.setString(kezdeti_string);

	// Beallitjuk a cursor poziciojat
	this->cursor_poz = this->text_box.findCharacterPos(kezdeti_string.length() - 1);
	this->cursor.setPosition(cursor_poz);
}

// User functions
void TextBox::process_event(const sf::Event& event)
{
	if (this->is_selected)										// Ha nincs kijelolve akkor nem irhatunk (vagy mozgathatjuk a cursort)
	{
		if (event.key.code == sf::Keyboard::Left)
		{
			this->process_arrow_input(true);					// Balra szeretnenk mozditani a cursor-t
		}
		else if (event.key.code == sf::Keyboard::Right)
		{
			this->process_arrow_input(false);					// Jobbra szeretnenk mozditani a cursor-t
		}
		else
		{
			int irt_karakter = event.text.unicode;
			if (irt_karakter < 128)
			{
				this->process_character(irt_karakter);			// Ebben az esetben pedig irni fogunk a cursor helyere
			}
		}
	}
}

void TextBox::draw_to_target(sf::RenderWindow& target)
{
	target.draw(this->text_box);

	if (this->is_selected)
	{
		target.draw(this->cursor);
	}
}
