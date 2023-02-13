#include "cache_jatek.h"

using namespace std;

int main()
{
	srand(static_cast<unsigned>(time(NULL)));

	CacheOrNoCash jatek("input.txt");

	while (jatek.is_running())
	{
		if (jatek.is_on_title()) jatek.title_screen_loop();

		// 'Frissitjuk' a jelenlegi frame-t
		jatek.update();

		// Megjelenitjuk a valtoztatasokat
		jatek.render();
	}
	cout << jatek.get_name() << "altal szerzett pontok: " << jatek.get_points();

	return 0;
}