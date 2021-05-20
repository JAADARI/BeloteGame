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
	cout << "Belote : nouveau jeu" << endl;
	cout << "Choisir le joueur qui commence la partie (0 - 3) : ";
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
	cout << " FELICITATIONS ! " << endl;
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

	IAauto monIAauto = IAauto(6);
	Hand tabHand[4];
	distribute8(tabHand);

	playGame(tabHand,monIAauto);

	while(1);
	return 0;
}