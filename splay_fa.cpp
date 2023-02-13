#include "splay_fa.h"

// << operator
template <class T>
ostream& operator<<(ostream& stream, const SplayFa<T>& fa)
{
	return fa.kiir(stream);
}

template <class T>
ostream& SplayFa<T>::kiir(ostream& stream) const
{
	stream << "Splay fa: ";

	// Eleinte 0 space van
	kiir_vertical(gyoker, 0, stream);

	return stream;
}

// class
template <class T>
void SplayFa<T>::rekurziv_felszabaditas(Node<T>* node)
{
	if (node->bal)
	{
		rekurziv_felszabaditas(node->bal);
	}
	if (node->jobb)
	{
		rekurziv_felszabaditas(node->jobb);
	}
	destroy_node(node);
}

template <class T>
SplayFa<T>::SplayFa(T* elemek, int db)
{
	this->gyoker = NULL;
	for (int elem = 0; elem < db; ++elem)
	{
		this->beszur(elemek[elem]);
	}
}

template <class T>
SplayFa<T>::SplayFa(const vector<T>& v)
{
	this->gyoker = NULL;
	for (int i = 0; i < v.size(); ++i)
	{
		this->beszur(v[i]);
	}
}

template <class T>
void SplayFa<T>::recursive_copy(Node<T>* current_node, Node<T>* copied_node)
{
	if (copied_node)
	{
		if (copied_node->bal)
		{
			current_node->bal = new Node<T>(copied_node->bal->adat);

			current_node->bal->parent = current_node;
			recursive_copy(current_node->bal, copied_node->bal);
		}
		else
		{
			current_node->bal = NULL;
		}

		if (copied_node->jobb)
		{
			current_node->jobb = new Node<T>(copied_node->jobb->adat);

			current_node->jobb->parent = current_node;
			recursive_copy(current_node->jobb, copied_node->jobb);
		}
		else
		{
			current_node->jobb = NULL;
		}
	}
}

template <class T>
SplayFa<T>::SplayFa(const SplayFa<T>& fa)
{
	this->gyoker = new Node<T>(fa.gyoker->adat);

	recursive_copy(gyoker, fa.gyoker);
}

// utility
template <class T>
void SplayFa<T>::destroy_node(Node<T>* node)
{
	delete node;
}

template <class T>
Node<T>* SplayFa<T>::find(Node<T>* node, T ertek) const
{
	if (node)
	{
		if (node->adat == ertek) return node;		// ha megtalaltuk a keresett elemet

		if (node->adat > ertek)
		{
			return find(node->bal, ertek);
		}
		else
		{
			return find(node->jobb, ertek);
		}
	}
	return NULL;
}

template <class T>
Node<T>* SplayFa<T>::find_parent(Node<T>* node, T ertek) const
{
	if (node)
	{
		if (node->adat == ertek) return NULL;				// ha mar letezik

		if (node->adat > ertek)
		{
			if (node->bal)
			{
				return find_parent(node->bal, ertek);
			}
			else
			{
				return node;
			}
		}
		else
		{
			if (node->jobb)
			{
				return find_parent(node->jobb, ertek);
			}
			else
			{
				return node;
			}
		}
	}
	return NULL;
}

template <class T>
Node<T>* SplayFa<T>::find_next(Node<T>* node) const
{
	if (node)
	{
		if (node->jobb) return find_min_in_subtree(node->jobb);					// ha letezik jobb elem, akkor visszateritjuk a legkisebb node-t a jobb reszfabol
		else if (node->parent)
		{
			Node<T>* szulo = node->parent;
			while (szulo && szulo->adat < node->adat)							// amig nem kapunk egy nagyobb elemet
			{
				szulo = szulo->parent;
			}

			return szulo;														// ha nem talalta meg itt is NULL lesz teritve
		}
	}
	return NULL;
}

template <class T>
Node<T>* SplayFa<T>::find_before(Node<T>* node) const
{
	if (node)
	{
		if (node->bal) return find_max_in_subtree(node->bal);					// ha letezik bal elem, akkor visszateritjuk a legnagyobb node-t a bal reszfabol
		else if (node->parent)
		{
			Node<T>* szulo = node->parent;
			while (szulo && szulo->adat > node->adat)
			{
				szulo = szulo->parent;
			}

			return szulo;														// ha nem talalta meg itt is NULL lesz teritve
		}
	}
	return NULL;
}

template <class T>
Node<T>* SplayFa<T>::find_min_in_subtree(Node<T>* node) const
{
	if (node)
	{
		if (node->bal) return find_min_in_subtree(node->bal);
		return node;
	}
	return NULL;
}

template <class T>
Node<T>* SplayFa<T>::find_max_in_subtree(Node<T>* node) const
{
	if (node)
	{
		if (node->jobb) return find_max_in_subtree(node->jobb);
		return node;
	}
	return NULL;
}

template <class T>
int SplayFa<T>::child_count(Node<T>* node) const
{
	int db = 0;
	if (node)
	{
		if (node->bal) db++;
		if (node->jobb) db++;
	}
	return db;
}

template <class T>
bool SplayFa<T>::is_leaf(Node<T>* node) const
{
	return (child_count(node) == 0);
}

template <class T>
void SplayFa<T>::delete_node(Node<T>* node)
{
	Node<T>* szulo = node->parent;

	if (is_leaf(node))											// ha level, akkor letorolhetjuk
	{
		if (szulo->bal && szulo->bal == node) szulo->bal = NULL;
		if (szulo->jobb && szulo->jobb == node) szulo->jobb = NULL;
		destroy_node(node);
		if (szulo) splay(szulo);
	}
	else
	{
		if (child_count(node) == 1)								// ha csak egy gyereke van, akkor feljebb csusztatjuk az egesz reszfat 1-el
		{
			if (szulo && szulo->bal && szulo->bal == node)		// megnezzuk melyik iranyban van amit torolunk
			{
				if (node->bal) szulo->bal = node->bal;			// szulo = szulo.gyerek.kov
				if (node->jobb) szulo->bal = node->jobb;
				destroy_node(node);
			}
			else
			{
				if (node->bal) szulo->jobb = node->bal;
				if (node->jobb) szulo->jobb = node->jobb;
				destroy_node(node);
			}
			if (szulo) splay(szulo);
		}
		else													// ebben az esetben 2 szulo van (valaszthatunk a jobb/bal reszfa minimum/maximum-ja kozott)
		{
			// ebben az implementacioban minimumot keresunk a jobb reszfaban
			Node<T>* min_in_right_subtree = find_min_in_subtree(node->jobb);	// min a jobb reszfaban
			int min_ertek = min_in_right_subtree->adat;			// lement ertek

			delete_node(min_in_right_subtree);					// meghivjuk a torlest erre az elemre (ez az elso 2 esetben fog tortenni)

			node->adat = min_ertek;								// 'bemasoljuk' az erteket az eredetileg torlendo elemhez
			if (szulo) splay(szulo);
		}
	}
}

template <class T>
void SplayFa<T>::preorder_recursive(Node<T>* node, vector<T>& nodes) const
{
	if (node)										// ha nem NULL
	{
		nodes.push_back(node->adat);				// feldolgozzuk a gyokerelemet

		preorder_recursive(node->bal, nodes);		// bejarjuk a bal oldali reszfat
		preorder_recursive(node->jobb, nodes);		// bejarjuk a jobb oldali reszfat
	}
}

template <class T>
void SplayFa<T>::inorder_recursive(Node<T>* node, vector<T>& nodes) const
{
	if (node)										// ha nem NULL
	{
		inorder_recursive(node->bal, nodes);		// bejarjuk a bal oldali reszfat

		nodes.push_back(node->adat);				// feldolgozzuk a gyokerelemet

		inorder_recursive(node->jobb, nodes);		// bejarjuk a jobb oldali reszfat
	}
}

template <class T>
void SplayFa<T>::postorder_recursive(Node<T>* node, vector<T>& nodes) const
{
	if (node)										// ha nem NULL
	{
		postorder_recursive(node->bal, nodes);		// bejarjuk a bal oldali reszfat
		postorder_recursive(node->jobb, nodes);		// bejarjuk a jobb oldali reszfat

		nodes.push_back(node->adat);				// feldolgozzuk a gyokerelemet
	}
}

template <class T>
void SplayFa<T>::zig(Node<T>* node)
{
	// Alaphelyzet:                 | 1. lepes:                              | 2. lepes:                      | 3. lepes:
	//                              |                                        |                                |
	//             nagyszulo		|                         nagyszulo      |                                |          nagyszulo
	//               | (ha van)     |                           |            |                                |         -> | (ha nem volt => gyoker = node)
	//             szulo            |           node          szulo          |           node  nagyszulo      |           node
	//             /   \            |          /   \       -> /   \          |          / -> \  |             |          /    \
	//           node szulo->jobb   | node->bal     node->jobb   szulo->jobb | node->bal     szulo            | node->bal     szulo
	//          /   \               |                                        |               /   \            |               /    \
	//  node->bal    node->jobb     |                                        |     node->jobb    szulo->jobb  |     node->jobb      szulo->jobb

	Node<T>* szulo = node->parent;
	if (szulo == NULL) throw RotationNotPossible();						// a forgatas nem lehetseges ha nincs szulo

	Node<T>* nagyszulo = szulo->parent;

	// 1. lepes:
	szulo->bal = node->jobb;											// a szulo->bal felveszi a node jobb reszfajat (minden elem aki kisebb mint szulo, de nagyobb mint node)
	if (szulo->bal) szulo->bal->parent = szulo;							// update node->jobb nagyszulo

	// 2. lepes:
	node->jobb = szulo;
	szulo->parent = node;

	// 3. lepes:
	node->parent = nagyszulo;
	if (nagyszulo)														// ha 'szulo'-nak letezett szuloje, akkor frissitjuk
	{
		if (nagyszulo->bal == szulo)									// megnezzuk, melyik gyereke volt eredetileg a 'szulo' es azt frissitjuk
		{
			nagyszulo->bal = node;
		}
		else
		{
			nagyszulo->jobb = node;
		}
	}
	else																// ha nem volt szuloje 'szulo'-nak (gyoker volt) akkor node lett az uj gyoker
	{
		gyoker = node;
	}
}

template <class T>
void SplayFa<T>::zag(Node<T>* node)
{
	// Alaphelyzet:                  | 1. lepes:                             | 2. lepes:                        | 3. lepes:
	//                               |                                       |                                  |
	//         nagyszulo		     |         nagyszulo                     |                                  |             nagyszulo
	//           | (ha van)          |           |                           |                                  |            -> | (ha nem volt => gyoker = node)
	//         szulo                 |         szulo        node             |     nagyszulo  node              |              node
	//         /   \                 |         /   \ <-    /   \             |            |  / <- \             |             /    \
	// szulo->bal    node            | szulo->bal   node->bal  node->jobb    |           szulo      node->jobb  |           szulo    node->jobb
	//              /   \            |                                       |          /    \                  |          /    \
	//      node->bal   node->jobb   |                                       | szulo->bal    node->bal          | szulo->bal       node->bal

	Node<T>* szulo = node->parent;
	if (szulo == NULL) throw RotationNotPossible();						// a forgatas nem lehetseges ha nincs szulo

	Node<T>* nagyszulo = szulo->parent;

	// 1. lepes:
	szulo->jobb = node->bal;											// a szulo->jobb felveszi a node bal reszfajat (minden elem aki nagyobb mint szulo, de kisebb mint node)
	if (szulo->jobb) szulo->jobb->parent = szulo;                       // update node->bal nagyszulo

	// 2. lepes:
	node->bal = szulo;
	szulo->parent = node;

	// 3. lepes:
	node->parent = nagyszulo;
	if (nagyszulo)													    // ha 'szulo'-nak letezett szuloje, akkor frissitjuk
	{
		if (nagyszulo->bal == szulo)									// megnezzuk, melyik gyereke volt eredetileg a 'szulo' es azt frissitjuk
		{
			nagyszulo->bal = node;
		}
		else
		{
			nagyszulo->jobb = node;
		}
	}
	else																// ha nem volt szuloje 'szulo'-nak (gyoker volt) akkor node lett az uj gyoker
	{
		gyoker = node;
	}
}

template <class T>
void SplayFa<T>::zigzig(Node<T>* node)
{
	Node<T>* szulo = node->parent;
	if (szulo == NULL) throw RotationNotPossible();			// a nagyszulot nem ellenorizzuk, ugyanis zig/zag leellenorzi

	zig(szulo);
	zig(node);
}

template <class T>
void SplayFa<T>::zagzag(Node<T>* node)
{
	Node<T>* szulo = node->parent;
	if (szulo == NULL) throw RotationNotPossible();			// a nagyszulot nem ellenorizzuk, ugyanis zig/zag leellenorzi

	zag(szulo);
	zag(node);
}

template <class T>
void SplayFa<T>::zigzag(Node<T>* node)
{
	Node<T>* szulo = node->parent;
	if (szulo == NULL) throw RotationNotPossible();			// a nagyszulot nem ellenorizzuk, ugyanis zig/zag leellenorzi

	// azert zag->zig a meghivas sorrendje, ugyanis a zig-t hamarabb el kell vegezni a gyereken
	zag(node);
	zig(node);
}

template <class T>
void SplayFa<T>::zagzig(Node<T>* node)
{
	Node<T>* szulo = node->parent;
	if (szulo == NULL) throw RotationNotPossible();

	// azert zig->zag a meghivas sorrendje, ugyanis a zag-t hamarabb el kell vegezni a gyereken
	zig(node);
	zag(node);
}

template <class T>
void SplayFa<T>::splay(Node<T>* node)
{
	if (gyoker != node)
	{
		Node<T>* szulo = node->parent;														// szulo biztos letezik, ugyanis csak akkor nem lenne szuloje, ha gyoker lenne
		Node<T>* nagyszulo = (szulo) ? szulo->parent : NULL;								// ha letezik nagyszulo felveszi, hanem NULL-t

		if (nagyszulo)
		{
			if (szulo->bal == node && nagyszulo->bal == szulo) zigzig(node);				// zigzig eset
			else if (szulo->jobb == node && nagyszulo->bal == szulo) zigzag(node);			// zigzag eset
			else if (szulo->bal == node && nagyszulo->jobb == szulo) zagzig(node);			// zagzig eset
			else if (szulo->jobb == node && nagyszulo->jobb == szulo) zagzag(node);			// zagzag eset
			else throw SplayFailed();
		}
		else
		{
			if (szulo->bal == node) zig(node);												// zig eset
			else if (szulo->jobb == node) zag(node);										// zag eset
			else throw SplayFailed();
		}

		splay(node);																		// rekurzivan meghivjuk ameddig a 'node' a gyoker nem lesz
	}
}

// user functions
template <class T>
void SplayFa<T>::beszur(T ertek)
{
	if (gyoker == NULL)															// ha meg nincs egy elem sem
	{
		gyoker = new Node<T>(ertek);											// a gyoker erre az elemre mutat (itt nem kell meghivni a splay muveletet)
	}
	else
	{
		Node<T>* szulo = find_parent(gyoker, ertek);								// atjarjuk a fat es megkeressuk az elotte kovetkezo node-t

		if (szulo != NULL)															// Mivel a jatek random szamokat general ezert kicsit valtoztatunk a beszurason, ha mar a faban van az ertek ignoraljuk
		{
			if (szulo->adat > ertek)												// megnezzuk, hova kellene beszurni
			{
				szulo->bal = new Node<T>(ertek);
				szulo->bal->parent = szulo;

				splay(szulo->bal);
			}
			else
			{
				szulo->jobb = new Node<T>(ertek);
				szulo->jobb->parent = szulo;

				splay(szulo->jobb);
			}
		}
	}
}

template <class T>
void SplayFa<T>::torol_keresofakent(T ertek)
{
	Node<T>* node = find(gyoker, ertek);

	if (node)
	{
		delete_node(node);
	}
	else
	{
		throw NotFound();
	}
}

template <class T>
void SplayFa<T>::torol_osszeillesztessel(T ertek)
{
	Node<T>* node = find(gyoker, ertek);				// alapertelmezetten a find metodus nem vegez splay muveletet
	splay(node);										// a node gyoker lesz

	SplayFa jobb;
	// ket SplayFa-ba osszuk a gyokeret (a jelenlegi objektum lesz a bal reszfa, es SplayFa jobb lesz a jobb reszfa)
	this->gyoker = node->bal;
	jobb.gyoker = node->jobb;

	// kitoroljuk az eredeti node-t
	destroy_node(node);

	// az eredeti gyokerre mutato pointereket NULL-ra allitjuk, ugyanis ezek a pointerek mostmar gyokerek
	this->gyoker->parent = NULL;
	jobb.gyoker->parent = NULL;

	osszeillesztes(jobb);
}

template <class T>
void SplayFa<T>::torol_osszeillesztessel(Node<T>* node)
{
	splay(node);										// a node gyoker lesz

	SplayFa jobb;
	// ket SplayFa-ba osszuk a gyokeret (a jelenlegi objektum lesz a bal reszfa, es SplayFa jobb lesz a jobb reszfa)
	this->gyoker = node->bal;
	jobb.gyoker = node->jobb;

	// kitoroljuk az eredeti node-t
	destroy_node(node);

	// az eredeti gyokerre mutato pointereket NULL-ra allitjuk, ugyanis ezek a pointerek mostmar gyokerek
	this->gyoker->parent = NULL;
	jobb.gyoker->parent = NULL;

	osszeillesztes(jobb);
}

template <class T>
void SplayFa<T>::szetvagas(SplayFa<T>& bal, SplayFa<T>& jobb)
{
	// a szetvagas muvelet elvegezheto tetszoleges elem eseten a splay(x) muvelet utan (x a gyokerbe kerul)
	// ez a metodus a gyoker szerint ossza ket reszfara, ezaltal nem is szukseges a splay(x) muvelet

	if (this->gyoker)
	{
		// a ket reszfa
		bal.gyoker = this->gyoker->bal;
		jobb.gyoker = this->gyoker->jobb;

		// toroljuk a jelenlegi objektum gyokeret
		destroy_node(this->gyoker);

		// a parent mutatokat NULL-ra allitjuk
		bal.gyoker->parent = jobb.gyoker->parent = NULL;

		// a jelenlegi objektumot 'toroljuk'
		this->gyoker = NULL;
	}
}

template <class T>
void SplayFa<T>::osszeillesztes(SplayFa<T>& fa)
{
	Node<T>* max_in_left_subtree = find_max_in_subtree(this->gyoker);				// a bal reszfa (jelenlegi objektum) legnagyobb eleme

	if (max_in_left_subtree)														// ha nem ures a jelenlegi objektum
	{
		splay(max_in_left_subtree);

		// a splay muvelete altal letrehoztunk egy 'baloldalas' fat, eszerint pedig a gyoker->jobb mindig szabad, ide becsatoljuk a fa reszfat
		this->gyoker->jobb = fa.gyoker;
	}
	else																			// ellenkezo esetben eleg ha a this->gyokeret az fa SplayFa gyokerere allitjuk
	{
		this->gyoker = fa.gyoker;
	}

	// 'toroljuk' a 'fa' reszfat
	fa.gyoker = NULL;
}

template <class T>
bool SplayFa<T>::letezik(T ertek)
{
	Node<T>* keresett = find(gyoker, ertek);

	if (keresett)
	{
		splay(keresett);
		return true;									// tehat megtalatuk es a gyokerbe 'emeltuk' az elemet
	}
	return false;
}

template <class T>
T SplayFa<T>::minimum() const
{
	if (gyoker) return find_min_in_subtree(gyoker)->adat;
	throw NotFound();
}

template <class T>
T SplayFa<T>::maximum() const
{
	if (gyoker) return find_max_in_subtree(gyoker)->adat;
	throw NotFound();
}

template <class T>
T SplayFa<T>::kovetkezo_elem(T ertek) const
{
	Node<T>* keresett_elotti_node = find(gyoker, ertek);				// node amelynek keressuk a kovetkezo elemet
	if (keresett_elotti_node)
	{
		Node<T>* kovetkezo = find_next(keresett_elotti_node);
		if (kovetkezo) return kovetkezo->adat;
		throw NotFound();
	}
}

template <class T>
T SplayFa<T>::elozo_elem(T ertek) const
{
	Node<T>* keresett_utani_node = find(gyoker, ertek);			// node amelynek keressuk az elozo elemet
	if (keresett_utani_node)
	{
		Node<T>* elozo = find_before(keresett_utani_node);
		if (elozo) return elozo->adat;
		throw NotFound();
	}
}

template <class T>
vector<T> SplayFa<T>::preorder_bejaras() const
{
	vector<T> nodes;

	preorder_recursive(gyoker, nodes);

	return nodes;
}

template <class T>
vector<T> SplayFa<T>::inorder_bejaras() const
{
	vector<T> nodes;

	inorder_recursive(gyoker, nodes);

	return nodes;
}

template <class T>
vector<T> SplayFa<T>::postorder_bejaras() const
{
	vector<T> nodes;

	postorder_recursive(gyoker, nodes);

	return nodes;
}

// +
template <class T>
void SplayFa<T>::kiir_vertical(Node<T>* node, int space, ostream& stream) const
{
	int count = 3;
	// Alap eset (ha levelhez ertunk visszaterunk)
	if (node == NULL) return;

	// Novel tavolsag
	space += count;

	// Feldolgozzuk a jobb gyereket (nagyobbak)
	kiir_vertical(node->jobb, space, stream);

	// Feldolgoz jelenlegi csomopont
	stream << '\n';
	for (int i = count; i < space; i++)
	{
		stream << ' ';
	}
	stream << node->adat << '\n';

	// Feldogolgoz bal gyerek (kisebbek)
	kiir_vertical(node->bal, space, stream);
}
