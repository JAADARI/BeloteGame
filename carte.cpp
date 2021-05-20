#include "carte.h"
#include <string>


Carte::Carte()
{
	value = -1;
	color = -1;
	point = -1;
	isAtout = false;
	ordre = -1;
}

Carte::Carte(int v, int c)
{
	value = v;
	color = c;
	isAtout = false;
	ordre = v;
	switch(v)
	{
	case SEPT:
	case HUIT:
	case NEUF:
		{
			point = 0;
			break;
		}
	case VALET:
		{
			point = 2;
			break;
		}
	case DAME:
		{
			point = 3;
			break;
		}
	case ROI:
		{
			point = 4;
			break;
		}
	case DIX:
		{
			point = 10;
			break;
		}
	case AS:
		{
			point = 11;
		}
	}
}

int Carte::getValue() const
{	
	return value;
}

int Carte::getOrdre() const
{	
	return ordre;
}

int Carte::getColor() const
{	
	return color;
}

int Carte::getPoint() const
{	
	return point;
}
void Carte::setAtout()
{
	isAtout = true;
	if(value == VALET)
	{
			point = 20;
			ordre = 10;
	}
	else if(value == NEUF) 
	{
			point = 14; 
			ordre = 9;
	}
}

bool operator==(const Carte& lhs, const Carte& rhs) 
{
    return lhs.getValue() == rhs.getValue() && lhs.getColor() == rhs.getColor();
}

bool operator<(const Carte& lhs, const Carte& rhs)
{
    if(lhs.getColor() < rhs.getColor())
		return true;
	else if (lhs.getColor() == rhs.getColor() && lhs.getValue() < rhs.getValue())
		return true;
	else 
		return false;
}

ostream& operator<<(ostream & str, Carte const & c)
{
	str << setw(6) << left;
	switch(c.getValue())
	{
	case Carte::SEPT:
		{
		str << "Sept";
		break;
		}
	case Carte::HUIT:
		{
		str << "Huit";
		break;
		}
	case Carte::NEUF:
		{
			str << "Neuf";
		break;
		}
	case Carte::VALET:
		{
			str << "Valet";
		break;
		}
	case Carte::DAME:
		{
			str << "Dame";
		break;
		}
	case Carte::ROI:
		{
			str << "Roi";
		break;
		}
	case Carte::DIX:
		{
			str << "Dix";
		break;
		}
	case Carte::AS:
		{
			str << "As";
		break;
		}
	}
	str << setw(8);
	switch(c.getColor())
	{
	case Carte::COEUR:
		{
		str << "Coeur";
		break;
		}
	case Carte::PIQUE:
		{
		str << "Pique";
		break;
		}
	case Carte::CARREAU:
		{
			str << "Carreau";
		break;
		}
	case Carte::TREFLE:
		{
			str << "Trefle";
		break;
		}
	}
	return str;
}

bool Carte::isSuperieur(Carte c)
{
	if(color == c.color) 
		return  ordre > c.ordre;
	else if(isAtout && !c.isAtout) 
		return true;
	else
		return false;

}