#include "Hand.h"
Hand::Hand()
{
}

Hand::Hand(Carte* c, int nb)
{
	nbCarte = nb;
	int j = 0;
	nbCarreau = 0;
	nbCoeur = 0;
	nbPique = 0;
	nbTrefle = 0;
	posCoeur = 0;
	int i=0;
	triABulle(c,nb);

	while(i<nbCarte)
	{
		while (c[i].getColor()== Carte::COEUR)
		{
			listHand[j]=c[i];
			nbCoeur++;
			j++;
			i++;
		}
		posPique = j;

		while (c[i].getColor()== Carte::PIQUE)
		{
			listHand[j]=c[i];
			nbPique++;
			j++;
			i++;
		}
		
		posCarreau = j;

		while (c[i].getColor()== Carte::CARREAU)
		{
			listHand[j]=c[i];
			nbCarreau++;
			j++;
			i++;
		}
		
		posTrefle = j;

		while (c[i].getColor()== Carte::TREFLE)
		{
			listHand[j]=c[i];
			nbTrefle++;
			j++;
			i++;
		}
	}
}

//pour loptimisation de jeux
void Hand::deleteCarte(int index)
{
	Carte tmp;
	tmp = listHand[index];
	for (int i=index;i<nbCarte-1;i++)
	{
		listHand[i] = listHand[i+1];
	}
	nbCarte--;

	switch (tmp.getColor())
	{
		case Carte::COEUR:
		{
		nbCoeur--;
		posPique--;
		posCarreau--;
		posTrefle--;
		break;
		}
	case Carte::PIQUE:
		{
		nbPique--;
		posCarreau--;
		posTrefle--;
		break;
		}
	case Carte::CARREAU:
		{
		nbCarreau--;
		posTrefle--;
		break;
		}
	case Carte::TREFLE:
		{
		nbTrefle--;
		break;
		}
	}
}

void Hand::deleteCarte(Carte c)
{
	for(int i=0; i<nbCarte;i++)
	{
		if(listHand[i]==c)
		{
			deleteCarte(i);
			break;
		}
	}
}

bool Hand::hasColor(int c)
{
	switch (c)
	{
		case Carte::COEUR:
		{
			return nbCoeur != 0;
		}
	case Carte::PIQUE:
		{
			return nbPique != 0;
		}
	case Carte::CARREAU:
		{
			return nbCarreau != 0;
		}
	case Carte::TREFLE:
		{
			return nbTrefle != 0;
		}
	default:
		{
			return false;
		}
	}
}

int Hand::nbColor(int c)
{
	switch (c)
	{
		case Carte::COEUR:
		{
			return nbCoeur;
		}
	case Carte::PIQUE:
		{
			return nbPique;
		}
	case Carte::CARREAU:
		{
			return nbCarreau;
		}
	case Carte::TREFLE:
		{
			return nbTrefle;
		}
	default:
		return -1;
	}
}

int Hand::posColor(int color)
{
	switch (color)
	{
		case Carte::COEUR:
		{
			return posCoeur;
		}
	case Carte::PIQUE:
		{
			return posPique;
		}
	case Carte::CARREAU:
		{
			return posCarreau;
		}
	case Carte::TREFLE:
		{
			return posTrefle;
		}
	default:
		return -1;
	}
}

Hand::~Hand(void)
{
}


void Hand::triABulle(Carte tableau[], int longueur)
{
   int i;
   bool permutation;

   do
   {
      permutation = false;
      for(i=0; i<longueur-1; i++)
      {
		  if(tableau[i].getColor()>tableau[i+1].getColor())
         {
			Carte tmp = tableau[i];
			tableau[i] = tableau[i+1];
			tableau[i+1] = tmp;
            permutation = true;
         }
      }
      longueur--;
   }
   while(permutation);
}

void Hand::triAtout(int color)
{
   int i;
   bool permutation;
   int pos = posColor(color);
   int longueur = (pos + nbColor(color)-1);
   do
   {
      permutation = false;
      for(i= pos; i<longueur && longueur > 0; i++)
      {
		  if(listHand[i].getOrdre()<listHand[i+1].getOrdre())
         {
			Carte tmp = listHand[i];
			listHand[i] = listHand[i+1];
			listHand[i+1] = tmp;
            permutation = true;
         }
      }
      longueur--;
   }
   while(permutation);
}

bool Hand::hasAtoutSup(int ordre, int atout){
	int pos = posColor(atout);
	return (listHand[pos].getOrdre() > ordre);
}

void Hand::setAtout(int atout)
{
	for(int i = 0; i < nbCarte; i++)
		if(listHand[i].getColor() == atout)
			listHand[i].setAtout();
}