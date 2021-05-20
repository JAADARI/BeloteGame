#pragma once
#include "paquet.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>



 //Class pour la gestion des cartes de chaque joueur

class Hand
{
public:
	Carte listHand[8];//array de carte dans chaque main 
	int nbCarte; 
	int posPique; //postion de premier pique
	int posCoeur; 
	int posCarreau;
	int posTrefle; 
	int nbPique; //nombre des cartes de type pique
	int nbCoeur; 
	int nbCarreau; 
	int nbTrefle; 

	
	Hand();
	
	//Simple constructeur, pour classifier le array par couleur 
 
	
	
	Hand(Carte*, int);

	// supprimer la carte a partir de son indice et reconstruire les carte du joueur sans la carte supprimer
	void deleteCarte(int index);

	
	//supprime la carte donner en parametre 
	void deleteCarte(Carte c);
	~Hand(void);

	//accesseur a la premier position du carte de couleur donné en parametre
	int posColor(int color);

	//tester si la main contient une carte du couleur passé en parametre
	bool hasColor(int c);

	//tester si la main contient une carte meilleure que celle en jeu 
	bool hasAtoutSup(int ordre, int atout);

// accesseur au nombre des carte dans la main ayant la couleur passer en parametre
	int Hand::nbColor(int c);

	// trier les cartes en atout avec couleur d'atout passé en parametre
	void triAtout(int color);


	void setAtout(int atout);

private:
// trier les carte dans la main ( tri d'un tableau de longeur passer aussi en param
	void triABulle(Carte tableau[], int longueur);

};

