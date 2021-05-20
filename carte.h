#ifndef CARTE_H
#define CARTE_H
#include <string.h>
#include <Windows.h>
#include <iostream>
#include <iomanip>

using namespace std;


// class Carte :  pour la construction  et l'utilisation des cartes


class Carte{
	int value; // Valeur (As, King, 8 ...) de carte 
	int color; //couleur de  (Pique, coeur ...) 
	int point; // nombre de points pour une carte 
	bool isAtout; //atout ou non
	int ordre; //orde de carte(Exemple As > 10 > 9 et 9 atout > AS > 10)
public : 
	enum color_type {COEUR, PIQUE, CARREAU, TREFLE};
	enum value_type {SEPT, HUIT, NEUF, VALET, DAME, ROI, DIX, AS};
	//constructeur vide 
	//qui initialise les attributs a -1
	Carte();
	
	// constructeur qui associe un entier  a valeur et couleur
 
	
	Carte(int, int);
	
	int getValue() const;
	
	int getColor() const;
	
	int getPoint() const;

	//Getter pour att ordre (aide a classifier les cartes)
	
	
	
	int getOrdre() const;

	
	bool isSuperieur(Carte c);
	
	
	void setAtout();

	// surcharge de l'operateur << pour l'affichage d'une carte
	friend ostream & operator<<(ostream & str, Carte const & c);
	
	friend bool operator==(const Carte& lhs, const Carte& rhs);
	//comparaison des couleurs si egale comparaison des valeurs
	friend bool operator<(const Carte& lhs, const Carte& rhs);
};

#endif