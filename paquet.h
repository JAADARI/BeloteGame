#ifndef PAQUET_H
#define PAQUET_H
#include <string.h>
#include <Windows.h>
#include <iostream>
#include "carte.h"

//Classe reprenant les 32 cartes d'un jeu

class Paquet{

public:
	int jeu [32];
	int nbCarte;
	Paquet();
};

#endif