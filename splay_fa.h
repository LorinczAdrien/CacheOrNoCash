#ifndef SPLAY_TREE
#define SPLAY_TREE

#include <iostream>
#include <vector>

using namespace std;

template <class T>
struct Node
{
	T adat;
	Node<T>* bal, * jobb;
	Node<T>* parent;

	Node(T ertek);
};

template <class T>
Node<T>::Node(T ertek)
{
	this->adat = ertek;
	this->bal = this->jobb = this->parent = NULL;
}

template <class T>
class SplayFa
{
	Node<T>* gyoker;

	void kiir_vertical(Node<T>* node, int space, ostream& stream) const;// kiir vertical seged fuggveny

	void destroy_node(Node<T>* node);									// felszabaditja egy node memoriajat
	void delete_node(Node<T>* node);									// binaris keresofaban valo torles (3 eset lekezelese) es kiegyensulyozas, ha kell
	void rekurziv_felszabaditas(Node<T>* node);							// destruktor seged fuggveny

	Node<T>* find(Node<T>* node, T ertek) const;						// pointer a keresett nodera
	Node<T>* find_parent(Node<T>* node, T ertek) const;					// pointer a keresett node szulojere (beszurasnal hasznaljuk)
	Node<T>* find_next(Node<T>* node) const;							// pointer a kovetkezo elemre
	Node<T>* find_before(Node<T>* node) const;							// pointer az elozo elemre
	Node<T>* find_min_in_subtree(Node<T>* node) const;					// minimumot keres egy adott reszfaban
	Node<T>* find_max_in_subtree(Node<T>* node) const;					// maximumot keres egy adott reszfaban

	void recursive_copy(Node<T>* current_node, Node<T>* copied_node);
	void preorder_recursive(Node<T>* node, vector<T>& nodes) const;
	void inorder_recursive(Node<T>* node, vector<T>& nodes) const;
	void postorder_recursive(Node<T>* node, vector<T>& nodes) const;

	bool is_leaf(Node<T>* node) const;									// a csomopont level -> TRUE, hanem FALSE
	int child_count(Node<T>* node) const;								// csomopont gyerekeinek szama (0, 1, 2)

	void splay(Node<T>* node);											// node-t a gyokerbe mozgatja zig, zag, zigzig, zagzag, zigzag es zagzig muveletekkel
	void zig(Node<T>* node);											// node->parent-t es node-t elforgatja jobbra (es reszfaikat megfeleloen beallitja)
	void zag(Node<T>* node);											// node->parent-t es node-t elforgatja balra (es reszfaikat megfeleloen beallitja)
	void zigzig(Node<T>* node);											// 1. elvegzi a zig muveletet node->parent-re 2. elvegzi a zig muveletet node-ra
	void zagzag(Node<T>* node);											// 1. elvegzi a zag muveletet node->parent-re 2. elvegzi a zag muveletet node-ra
	void zigzag(Node<T>* node);											// 1. elvegzi a zig muveletet node-ra 2. elvegzi a zag muveletet node-ra
	void zagzig(Node<T>* node);											// 1. elvegzi a zag muveletet node-ra 2. elvegzi a zig muveletet node-ra
	void torol_osszeillesztessel(Node<T>* node);						// osszeillesztessel torol egy node-t

public:

	class ItemAlreadyInTree {};
	class NotFound {};
	class NonValidIndex {};
	class RotationNotPossible {};
	class SplayFailed {};

	SplayFa() { gyoker = NULL; }
	SplayFa(T* elemek, int db);
	SplayFa(const vector<T>& v);
	SplayFa(const SplayFa& fa);
	~SplayFa() { if (gyoker) rekurziv_felszabaditas(gyoker); };

	void beszur(T ertek);												// erteket csatol hozza, ha mar letezik az ertek, 'throw ItemAlreadyInTree'
	void torol_keresofakent(T ertek);									// megkeresi, illetve torli az adott erteket, ha nem letezik, 'throw NotFound'
	void torol_osszeillesztessel(T ertek);
	void torol_gyoker() { torol_osszeillesztessel(gyoker); }

	T minimum() const;													// keresofaban a maximum
	T maximum() const;													// keresofaban a minimum
	T elozo_elem(T ertek) const;										// elozo elem erteke, ha letezik, hanem 'throw NotFound'
	T kovetkezo_elem(T ertek) const;									// kovetkezo elem erteke, ha letezik, hanem 'throw NotFound'
	bool letezik(T ertek);												// ha letezik a keresofaban az ertek

	void szetvagas(SplayFa& bal, SplayFa& jobb);						// a jelenlegi objektumot ket reszfaba 'vagja' (SplayFak a reszfak is) es 'torli' a jelenlegi objektumot
	void osszeillesztes(SplayFa& fa);									// osszeilleszti a jelenlegi objektumot egy adott SplayFa-val (az adott objektum 'torlodik')

	vector<T> preorder_bejaras() const;									// preorder bejaras -> csomopontok (adatai)
	vector<T> inorder_bejaras() const;									// preorder bejaras -> csomopontok (adatai)
	vector<T> postorder_bejaras() const;								// preorder bejaras -> csomopontok (adatai)

	ostream& kiir(ostream& stream) const;								// cout << BinarisKeresofa operatorhoz
};

#include "splay_fa.cpp"

#endif


