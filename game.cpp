#include <Windows.h>
#include <iostream>
#include <iomanip>
#include "carte.h"
#include "paquet.h"
#include <string>
#include "Hand.h"
#include "IAauto.h"
#include "ConsoleColor.h"
#include <map>

using namespace std;

#define NBFORIA 15

void printAtout(int atout)
{
	switch(atout)
	{
	case -1:
		{
			cout << "Ne prend pas" << endl;
			break;
		}
	case 0:
		{
			cout << "Coeur" << endl;
			break;
		}
	case 1:
		{
			cout << "Pique" << endl;
			break;
		}
	case 2:
		{
			cout << "Carreau" << endl;
			break;
		}
	case 3:
		{
			cout << "Trefle" << endl;
			break;
		}
	}
}

int gestionPrise(Hand* tabHand,int firstPlayer, int playerIA,IAauto monIA)
{
	int atout = 1;

	int resIa[NBFORIA];
	IAauto tabIa[NBFORIA];
#pragma omp parallel for
	for (int iForIa = 0; iForIa <NBFORIA; iForIa++)
	{
		srand(iForIa);
		tabIa[iForIa] = IAauto(monIA.nbTour);
		resIa[iForIa] = tabIa[iForIa].prendre(tabHand[playerIA], firstPlayer, playerIA);
	}
	map<int, int> m;

	for (int iForIa = 0; iForIa <NBFORIA; iForIa++)
	{
		int tmp = resIa[iForIa];
		if(m.find(tmp).operator==(m.end()))
		{
			m[tmp]=0;
		} 
		m[tmp]++;
	}
	int max = -1;
	int tmpForIa;
	for (std::map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
	{
		if (it->second > max)
		{
			tmpForIa = it->first;
			max = it->second;
		}
	}

	int atoutPris = tmpForIa;
	
	if(atoutPris != -1)
	{
		atout = atoutPris;
		cout << "Atout pris par l'IA = ";
		printAtout(atoutPris);
	}
	else
		cout << "Atout par defaut : pique" << endl;
	for(int i = 0; i < 4; i++)
	{
		tabHand[i].setAtout(atout);
		tabHand[i].triAtout(atout);
	}
	return atout;
}

void playGame(Hand htab[4],IAauto monIA)
{	
	int player, carteJoue, colorPlay, winner, scorePli, scoreA, scoreB;
	winner = -1;
	cout << "\n\n\n\n";
	cout << "_______________________________________________ BELOTE : NOUVEAU JEU __________________________________________________" << endl;
	cout << "\n\n";
	cout << "                                 Choisir le joueur qui commence la partie (1 - 3) : ";
	cin >> player;
	int atout = gestionPrise(htab, player,0, monIA);
	scoreA = 0;
	scoreB = 0;
	Carte bCard;

	for(int nbTour = 1; nbTour < 9; nbTour++)
	{
		if (nbTour==3)
		{
			monIA.nbTour = 0;
		}
		cout << setfill('-');
		cout << setw(71) << "" << endl;
		cout << setfill(' ');
		cout << ".... Tour " << nbTour << " ...." << endl;
		monIA.printGame(htab);
		Carte c [4];
		scorePli = 0;
		bool valide;

		for (int i=0; i <4; i++)
		{	
			valide = false;
			if(i == 0 || i == 2) cout << blue <<".... Joueur " << player << ", (" << i+1 << "/4) ...." <<endl;
			else cout << yellow <<".... Joueur " << player << ", (" << i+1 << "/4) ...." <<endl;
			if (player == 0 && i == 0)
			{
				winner = player;
				Carte resIa[NBFORIA];
				IAauto tabIa[NBFORIA];
#pragma omp parallel for
				for (int iForIa = 0; iForIa <NBFORIA; iForIa++)
				{
					srand(iForIa);
					tabIa[iForIa] = IAauto(monIA.nbTour);
					resIa[iForIa] = tabIa[iForIa].nextCarte(htab[player],atout,player,0,Carte(),-1,0,winner);
				}
				map<Carte, int> m;

				for (int iForIa = 0; iForIa <NBFORIA; iForIa++)
				{
					Carte tmp = resIa[iForIa];
					if(m.find(tmp).operator==(m.end()))
					{
						m[tmp]=0;
					} 
					m[tmp]++;
				}
				int max = -1;
				Carte tmpForIa;
				for (std::map<Carte, int>::iterator it = m.begin(); it != m.end(); ++it)
				{
					if (it->second > max)
					{
						tmpForIa = it->first;
						max = it->second;
					}
				}
				bCard = tmpForIa;
				cout << "La carte jouee est : " << bCard << endl;
				scorePli += bCard.getPoint();
				c[i] = bCard;
				htab[player].deleteCarte(bCard);
				colorPlay = bCard.getColor();
			}
			else if(i == 0)
			{
				cout << "Joue quoi ? ";
				cin >> carteJoue;

				if(carteJoue < htab[player].nbCarte)
				{
					Carte tmp=htab[player].listHand[carteJoue];

					valide = monIA.isCarteValide(htab[player],tmp,-1,atout,bCard,winner,player);
					while(!valide)
					{
						cout << "Carte non acceptee selon les regles du jeu ! " << endl;
						cout << "Joue quoi ? ";
						cin >> carteJoue;
						tmp=htab[player].listHand[carteJoue];
						valide = monIA.isCarteValide(htab[player],tmp,-1,atout,bCard,winner,player);
					}
					bCard = tmp;
					winner = player;
				}
				else
				{
					bCard = monIA.carteAuto(htab[player],-1,atout,bCard,winner,player);
				}

				colorPlay = bCard.getColor();
				cout << "La carte jouee est : " << bCard << endl;
				scorePli += bCard.getPoint();
				c[i] = bCard;
				htab[player].deleteCarte(bCard);
			}
			else if (player == 0)
			{
				Carte resIa[NBFORIA];
				IAauto tabIa[NBFORIA];
#pragma omp parallel for
				for (int iForIa = 0; iForIa <NBFORIA; iForIa++)
				{
					srand(iForIa);
					tabIa[iForIa] = IAauto(monIA.nbTour);
					resIa[iForIa] = tabIa[iForIa].nextCarte(htab,atout,player,i,bCard,colorPlay,0,winner);
				}
				map<Carte, int> m;

				for (int iForIa = 0; iForIa <NBFORIA; iForIa++)
				{
					Carte tmp = resIa[iForIa];
					if(m.find(tmp).operator==(m.end()))
					{
						m[tmp]=0;
					} 
					m[tmp]++;
				}
				int max = -1;
				Carte cardToPlay;
				for (std::map<Carte, int>::iterator it = m.begin(); it != m.end(); ++it)
				{
					if (it->second > max)
					{
						cardToPlay = it->first;
						max = it->second;
					}
				}

				
				cout << "La carte jouee est : " << cardToPlay << endl;
				scorePli += cardToPlay.getPoint();
				bool sup;
				sup = cardToPlay.isSuperieur(bCard);
				cout << cardToPlay << "est superieur a " << bCard << " ? => " << sup <<endl;
				if (sup)
				{
					bCard=cardToPlay;
					winner = player;
				}
				c[i]= cardToPlay;
				htab[player].deleteCarte(cardToPlay);
			}
			else {
				cout << "Joue quoi ? ";
				cin >> carteJoue;
				Carte tmp;

				if(carteJoue < htab[player].nbCarte)
				{
					tmp = htab[player].listHand[carteJoue];

					valide = monIA.isCarteValide(htab[player],tmp,colorPlay,atout,bCard,winner,player);
					while(!valide)
					{
						cout << "Carte non acceptee selon les regles du jeu ! " << endl;
						cout << "Joue quoi ? ";
						cin >> carteJoue;
						tmp=htab[player].listHand[carteJoue];

						valide = monIA.isCarteValide(htab[player],tmp,colorPlay,atout,bCard,winner,player);
					}
				}
				else
				{
					tmp = monIA.carteAuto(htab[player],colorPlay,atout,bCard,winner,player);
				}


				cout << "La carte jouee est : " << tmp << endl;
				scorePli += tmp.getPoint();
				bool sup;
				sup = tmp.isSuperieur(bCard);
				cout << tmp << "est superieur a " << bCard << " ? => " << sup <<endl;
				if (sup)
				{
					bCard=tmp;
					winner = player;
				}
				c[i] = tmp;
				htab[player].deleteCarte(tmp);
			}
			player = (player +1)%4;
		}
		monIA.delListCard(c, 4);
		if(nbTour < 4) 
			monIA.nbTour--;
		if(nbTour == 8) 
			scorePli +=10;
		cout << white << "Le gagnant du pli est : Joueur " << winner << " - Score du pli : " << scorePli << endl;
		if(winner == 0 || winner == 2) 
			scoreA += scorePli;
		else 
			scoreB += scorePli;
		player = winner;
	}
	cout << "La partie est terminee" << endl;
	if(scoreA > scoreB)
		cout << "Les gagnants sont les Joueurs 0 et 2, avec un score final de " << scoreA << " points" << endl;
	else
		cout << "Les gagnants sont les Joueurs 1 et 3, avec un score final de " << scoreB << " points" << endl;
	cout << "\n\n\n";

	cout << "              _____________________________ F E L I C I T A T I O N S   ! _______________________________ " << endl;

	
	
}

void distribute8(Hand* tabHand)
{
	vector<Carte> cartesRestantes;

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<8; j++)
		{
			Carte c = Carte(j,i);
			cartesRestantes.push_back(c);
		}
	}

	for(int i = 0; i < 4; i++)
	{
		Carte newHand[8];
		for(int j = 0; j < 8; j++)
		{
			int posCarte = rand() %cartesRestantes.size();
			Carte tmp = cartesRestantes[posCarte];
			
			newHand[j] = tmp;
			cartesRestantes.erase(cartesRestantes.begin() + posCarte);
		}
		tabHand[i] = Hand(newHand,8);
		
	}
}




int main()  {

	srand(time(NULL));
	int x;
	string y;
	IAauto monIAauto = IAauto(6);
	Hand tabHand[4];
	do
	{


		cout << "\n\n\n\n";
		cout << "            >>>>>>>>>>>>  |_|_|_|    |_|_|_|_|  |_|          |_|_|    |_|_|_|_|_|  |_|_|_|_|  <<<<<<<<<<<<" << "\n";
		cout << "            >>>>>>>>>>>>  |_|   |_|  |_|        |_|        |_|   |_|      |_|      |_|        <<<<<<<<<<<<" << "\n";
		cout << "            >>>>>>>>>>>>  |_|_|_|    |_|_|_|    |_|        |_|   |_|      |_|      |_|_|_|    <<<<<<<<<<<<" << "\n";
		cout << "            >>>>>>>>>>>>  |_|   |_|  |_|        |_|        |_|   |_|      |_|      |_|        <<<<<<<<<<<<" << "\n";
		cout << "            >>>>>>>>>>>>  |_|_|_|    |_|_|_|_|  |_|_|_|_|    |_|_|        |_|      |_|_|_|_|  <<<<<<<<<<<<" << "\n\n\n";
		
		cout << "________________________________________________________________________________________________________________________" << "\n";
		cout << "                                     ____________________________________________" << "\n\n\n";

		cout << "                                               | 1 : START NEW GAME !" << "\n\n";
		cout << "                                               | 2 : REGLE DE JEU " << "\n\n";
		cout << "                                               | 3 : A PROPOS" << "\n\n";
		cout << "                                               | 4 : QUITTER" << "\n\n\n";
		cout << "                                     ____________________________________________" << "\n";
		cout << "________________________________________________________________________________________________________________________" << "\n\n\n";

		cin >> x;

		switch (x)
		{
		case 1:
			system("CLS");


			distribute8(tabHand);

			playGame(tabHand, monIAauto);
			break;
		case 3:
			system("CLS");
			cout << "\n\n";
			cout << "____________________________________________________B E L O T E V1.0_____________________________________________________" << "\n";

			cout << "                                       Jeux de BELOTE V1.0 realise par : " << "\n\n";
			cout << "                                                 I S S A    T A L E B" << "\n\n";
			cout << "                                                 J A A D A R I    F I R A S" << "\n\n";
			cout << "                                                 I S S A M    H E D H L I" << "\n\n";
			cout << "                                       Encadre par madame: " << "\n\n\n";
			cout << "                                                 B O U K H R I S   I M E N " << "\n\n\n";

			cout << "                        _______________________________________________________________________________" << "\n\n";
			cout << "                           N a t i o n a l   S c h o o l   f o r   C o m p u t e r   S c i e n c e s    " << "\n";
			cout << "                        _______________________________________________________________________________ " << "\n\n\n\n";
			cout << "                                                    2 0 2 0 / 2 0 2 1 " << "\n";
			
			break;
		case 2:
			system("CLS");

			cout << "DESCRIPTION DE BELOTE :" << "\n\n\n";
			cout << "La belote est un jeu de contrat qui se joue par equipe de deux." << "\n";
			cout << "Les plis remportes par les joueurs d’une meme equipe sont donc a additionner." << "\n";
			cout << "A la fin dune partie, on compte le nombre de points des cartes contenus dans les plis remportes. " << "\n";
			cout << "On sait alors si lequipe qui a pris est dedans ou a rempli son contrat." << "\n\n\n\n";
			cout << "Pour jouer a la Belote, il vous faut :" << "\n\n";
			cout << "- etre 4 joueurs" << "\n";
			cout << "- Un jeu de cartes classique de 32 cartes." << "\n\n\n\n";
			cout << "Commencer une partie de Belote :" << "\n\n";
			cout << "Le joueur qui se trouve a gauche du donneur debute la manche." << "\n";
			cout << "Chaque joueur est dans l obligation de suivre la couleur demandee. " << "\n";
			cout << "Si un joueur ne peut pas fournir de carte de la couleur demandee il doit alors couper à l atout. " << "\n";
			cout << "Au cas ou il n a pas d atout, il doit pisser c est-a-dire jouer une autre carte dans une autre couleur." << "\n";
			cout << "Il est egalement possible de ne pas jouer de l atout dans le cas ou" << "\n";
			cout << "le partenaire est maître du pli alors que l on ne possède pas de cartes de la couleur demandee." << "\n";
			cout << "Le gagnant d’un pli le ramasse et débute le prochain tour. " << "\n";
			cout << "Quand le cas ou deux joueurs coupent, le second joueur doit obligatoirement surcouper," << "\n";
			cout << " c est-a-dire fournir un atout plus fort que celui qui est deja sur la table." << "\n";
			cout << "C est le cas également pour les autres joueurs suivant." << "\n";
			cout << "Dans le cas ou le joueur n a pas d’atout plus fort," << "\n";
			cout << "il doit quand meme jouer un atout plus bas si bien sur il en possede un dans son jeu." << "\n";

			break;




		case 4: exit(0);
			break;
		default:
			break;
		}
		cout << "\n\n\n\n\n\n";
		cout << "                        ______________   Vous voulez revenir au menu principal (yes/no) ?   ______________ " << "\n";
		cin >> y;
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	}
	while (y=="yes");

	exit(0);
	while(1);
	return 0;
}