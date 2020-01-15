#include "RPG Game.h"
#include <string>
using namespace std;

Item::Item(string name, int attack, int defense, int weight)
{
	this->name = name;
	this->attack = attack;
	this->defense = defense;
	this->weight = weight;
}

Item::Item()
{
	this->name = "";
	this->attack = 0;
	this->defense = 0;
	this->weight = 0;
}

string Item::getName()
{
	return name;
}

int Item::getAttack()
{
	return attack;
}

int Item::getDefense()
{
	return defense;
}

int Item::getWeight()
{
	return weight;
}