#include "cpu_data.h"

// Package
Package::Package(int size, int priority)
{
	this->place = 0;
	this->size = size; this->priority = priority;
	this->is_on_screen = this->used = false;

	// Shape
	this->shape.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
	this->shape.setRadius(static_cast<float>(rand() % 20 + 20));
	this->shape.setOutlineThickness(2);
	this->shape.setOutlineColor(sf::Color::Black);

	// Elements
	this->elements.resize(size);
	for (int i = 0; i < size; ++i) this->elements[i] = static_cast<double>(rand() % 1000);
}

CPU_DATA::CPU_DATA(string input_file, int L1_size, int L2_size)
{
	this->L1_max = L1_size; this->L2_max = L2_size;
	this->L1_size = this->L2_size = this->L3_size = 0;

	ifstream fin(input_file);

	int csomag_db = 0, size, priority;
	fin >> csomag_db;
	for (int i = 0; i < csomag_db; ++i)
	{
		fin >> size >> priority;
		this->packages.push_back(Package(size, priority));
	}

	fin.close();
}

void CPU_DATA::process_package(int package_ind, int place_ind, int& player_points, string& gained)
{
	stringstream stream;
	if (package_ind >= 0 && package_ind < this->packages.size())
	{
		int package_count = this->packages[package_ind].size;
		int won_points = 0;
		switch (place_ind)
		{
		case 4:
			// L1 cache multiplier 100%
			won_points = static_cast<double>(this->packages[package_ind].priority);

			if (this->L1_size + package_count > this->L1_max)
			{
				player_points -= won_points;
				stream << "- " << won_points;
			}
			else
			{
				int i = 0;
				for (int jelen_size = this->L1_size; jelen_size < this->L1_size + package_count; ++jelen_size)
				{
					L1_cache[jelen_size] = this->packages[package_ind].elements[i++];
				}
				this->L1_size += package_count;

				// Update player points
				player_points += won_points;
				stream << "+ " << won_points;
			}
			break;

		case 5:
			// L2 cache multiplier 50%

			won_points = static_cast<double>(this->packages[package_ind].priority) * 0.5;

			if (this->L2_size + package_count > this->L2_max)
			{
				player_points -= won_points;
				stream << "- " << won_points;
			}
			else
			{
				int i = 0;
				for (int jelen_size = this->L2_size; jelen_size < this->L2_size + package_count; ++jelen_size)
				{
					L2_cache[jelen_size] = this->packages[package_ind].elements[i++];
				}
				this->L2_size += package_count;
				player_points += won_points;
				stream << "+ " << won_points;
			}
			break;

		case 6:
			// L3 cache multiplier 20%
			won_points = static_cast<double>(this->packages[package_ind].priority) * 0.2;
			player_points += won_points;
			stream << "+ " << won_points;

			int i = 0;
			for (int jelen_size = this->L3_size; jelen_size < this->L3_size + package_count; ++jelen_size)
			{
				L3_cache.beszur(this->packages[package_ind].elements[i++]);
			}
			this->L3_size += package_count;

			break;
		}
		gained = stream.str();
	}
}

void CPU_DATA::create_new_package()
{
	int random = rand() % 10;	// 0 - 9
	if (random >= 0 && random <= 4)										// 50% esely, hogy L1-bol probal uj packaget kesziteni
	{
		if (this->L1_size >= (this->L1_max / 2))
		{
			int uj_package_meret = rand() % this->L1_size + 1;			// egy szam [1, L1_size]
			int uj_priority = rand() % (uj_package_meret * 2) + uj_package_meret;

			L1_cache.erase(L1_cache.find(this->L1_size - uj_package_meret), L1_cache.find(this->L1_size));
			this->L1_size -= uj_package_meret;

			this->packages.push_back(Package(uj_package_meret, uj_priority));
		}
	}
	else if (random >= 5 && random <= 8)								// 40% esely hogy L2-bol probal uj package kesziteni
	{
		if (this->L2_size >= (this->L2_max / 2))
		{
			int uj_package_meret = rand() % this->L2_size + 1;			// egy szam [1, L1_size]
			this->L2_size -= uj_package_meret;
			int uj_priority = rand() % (uj_package_meret * 2) + uj_package_meret;

			L2_cache.erase(L2_cache.find(this->L2_size - uj_package_meret), L2_cache.find(this->L2_size));
			this->L2_size -= uj_package_meret;

			this->packages.push_back(Package(uj_package_meret, uj_priority));
		}
	}
	else																// 10% esely hogy L3-bol probal uj package-t kesziteni
	{
		if (this->L3_size)
		{
			int uj_package_meret = rand() % this->L3_size + 1;			// egy szam [1, L3_size]
			this->L3_size -= uj_package_meret;
			int uj_priority = rand() % (uj_package_meret * 2) + uj_package_meret;

			// 'Felhasznaljuk' a leghasznaltabb elemeket
			for (int i = 0; i < uj_package_meret; ++i)
			{
			
				this->L3_cache.torol_gyoker();
			}

			// A processzor tevekenysege altal letrejonnek uj elemek
			this->packages.push_back( Package(uj_package_meret, uj_priority) );
		}
	}
}
