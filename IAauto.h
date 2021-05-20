#pragma once
#include "Hand.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
//class IAauto : deroulement du jeu automatique pour l'ordinateur
class IAauto
{
public:
	int player; // ID pour le joueur  the id of the main player in this round. It will be the player we want to maximise his score*/
	Hand h; //main du joueur principal
	int partner; //joueur ami du joueur principal
	int nbFin;//compteur pour savoir le nombre d'operation a faire
	int cpt; //
	int cardsStillInGame[32]; // tableau des carte restant dans le jeu (1: valable , 0 : non)
	int nbCardDeleted; //compteur des carte supprimées du jeu
	int nbTour; // compteur pour savoir quand l'IA etablie toutes les possibilités : nombre des tour pour l'IA joue toute les carte du joueur 

	struct state //state utilisé et transmis dans le jeu
	{
		int nbPlayerPli; //nombre du joueur qui joue durant le pli
		int playerHasHand; //joueur ayant la main
		int playerWiningPli; //joueur gagnant le plis
		Carte BestCard; //meilleure carte jouée jusqu'a present
		int colorAsk; //couleur que le joueur doit jouer
		int atout; // couleur atout
		Hand currentHand; //main du joueur actuel
		Hand allHand[4]; // tableau ayant tout les main dans le jeu actuel
		int nbCarte; // nombre des carte que le joueur actuelle possede en main
		int valuePli; // Valeur du  ce Pli
		int score; // score du jeu dans ce state
	};

	
	// Fonction qui lance et teste l'algorithme minimax, un state initial va etre crée
	// les inputs : 
	//				firstPlayer : nombre du joueur qui va jouer le premie
	//				atout       : couleur choisi come atout
	//				Hand *		: tableau des 4 mains
	// Output		:	
	//				retourner le score de l'algorithme
	int minimax(int firstPlayer, int atout, Hand *);
	
	
	// fonction intermediaire appelée dans minimax algorithme
	//	Input   : state c : state actuelle du jeu 
	// Output	: score de l'algorithme
	
	int minimax(state c);
	
	// fonction intermediaire appelée dans minimax algorithm
	//		Input : state c : state actuelle du jeu 
	// Output	:	score de l'algorithme
	int maxValue(state c);
	
	//fonction intermediaire dans minimax algorithm
	int minValue(state c);
	// Fonction pour lancer et tester l'algorithme minimax , un state initiale sera crée, cet algo est utilisé avec la taille d'arbre des possibilités en utilisant alpha et beta
	// Input :	
	//			firstPlayer : nombre du 1er joueur qui joue le premier 
	//			atout :			couleur d'atout 
	//			Hand*	:		tableau de 4 Hand est necessaire
	// Output	:	
	//					Score de l'algorithme

	int minimaxAlphaBeta(int firstPlayer, int atout, Hand *);
	/**
	Fonction intermediaire appelée dans le minimax algo
	//
	
	@param[in] le stat actuel de jeu
	@param[in] alpha integer avec le min val sauvegarder
	@param[in] beta integer avec le max val sauvegarder
	@return score  de lalgo
	*/
	int minimax(state c, int alpha, int beta);
	/**
	
	 Fonction intermédiaire appelée dans l'algorithme minimax
     c: c l'état réel du jeu
     alpha: entier alpha avec la valeur minimale enregistrée
     beta: bêta entier avec la valeur maximale enregistrée

	*/
	int maxValue(state c, int alpha, int beta);
	//meme cmnt
	int minValue(state c, int alpha, int beta);
	
	/**
	Fonction intermédiaire appelée dans l'algorithme minimax pour creation de new stat
	     c: c l'état réel du jeu

	 i position de la carte a jouee dans la main 
	return new state
	*/
	state majState(state c, int i);

	/**
	Fonction intermédiaire appelée dans l'algorithme minimax pour supp une carte
	c une carte a supp
	*/
	void deleteCard(Carte c);

	
	//Cette fonction donnée à la main d'un joueur décidera quelle carte jouer en utilisant l'algorithme minimax alpha beta

	Carte nextCarte(Hand, int atout, int player, int numberCardPlayedInPli, Carte bestCard, int colorAsk, int valuePli, int playerWining);
	
//Cette fonction donnée à la main d'un joueur décidera quelle carte jouer en utilisant l'algorithme minimax alpha beta
	
	Carte nextCarte(Hand*, int atout, int player, int numberCardPlayedInPli, Carte bestCard, int colorAsk, int valuePli, int playerWining);

	/**
	fonction dans nextcarte pour faire la distrubition 
    htab nouveau tableau de Hand avec la carte de l'autre joueur assignée au hasard

	atout : atout de jeu 
	*/
	void distributionCards(Hand h, Hand[4], int atout);

	/**
fonction pou laffichage de jeu 
	*/
	void printGame(Hand htab[4]);

	/**
	Intermediary function pour supp qlq cartes
	c an array de cartes
	lg longueur pour supp qlq cartes
	*/
	void delListCard(Carte c[], int lg);

	/**
	Fonction utilisée pour nous informer si une  carte est valide
	h main de joueur
	ccarte a testee
	 colorAsk couleur que le joueur doit jouer ce tour
	 atout atout de jeu
	bestCard best card played during this turn
	winner the player actually wining the turn
	actualPlayer who is actually playing
	@return true iif we can play this card
	*/
	bool isCarteValide(Hand h, Carte c, int colorAsk, int atout, Carte bestCard, int winner, int actualPartner);

	
	Carte carteAuto(Hand h,int colorAsk,int atout, Carte bestCard,int winner,int actualPlayer);

	
	int prendre(Hand h,int firstPlayer, int playerIA);

	int prendreScore(Hand h, int atout, int firstPlayer, int playerIA);
	/**
	 function appelee dans prendre algorithm pour la creation de nouvelle distribution des cartes
	
	*/
	void distributionPrise(Hand h, Hand* htab,int atout);
	
	/**
	Simple constructeur
	@return new IAvDream
	*/
	IAauto();

	/**
	Simple constructeur pour initialiser les paras
 
	@param[in]     nb nombre de tours que toutes les possibilités devront être évaluées à l'intérieur de l'algorithme minimax
	@return Hand
	*/
	IAauto(int nb);
	~IAauto(void);
};

