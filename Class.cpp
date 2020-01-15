#include "RPG Game.h"
#include <iostream>
#include <string>

void Class::levelUp()
{
	level++;
	maxHealth += healthPerLevel;
}

void Class::regainHealth(int healing)
{
	health += healing;
	if (health > maxHealth) //Don't go over max health
		health = maxHealth;
}

void Class::takeDamage(int hurt)
{
	health -= hurt;
}

int Class::getLevel()
{
	return level;
}

int Class::getHealth()
{
	return health;
}

int Class::getMaxHealth()
{
	return maxHealth;
}

std::string Class::getName()
{
	return className;
}

double Class::getAccuracy()
{
	return accuracy;
}

Class::Class(int Maximum, int perLevel, std::string name, double acc)
{
	level = 1;
	maxHealth = Maximum;
	healthPerLevel = perLevel;
	health = Maximum;
	className = name;
	accuracy = acc;
}

void Class::display()
{
	std::cout << className << " class:" << std::endl;
	std::cout << "Starting Health: " << maxHealth << std::endl;
	std::cout << "Health gained per level: " << healthPerLevel << std::endl;
	std::cout << std::endl;
}

Barbarian::Barbarian() : Class(30, 12, "Barbarian", 0.7)
{
}

Barbarian::~Barbarian()
{
	std::cout << "oof" << std::endl;  // All of these don't really do anything...
}

Fighter::Fighter() : Class(25, 10, "Fighter", 0.8)
{
}

Fighter::~Fighter()
{
	std::cout << "oof" << std::endl;
}

Duelist::Duelist() : Class(25, 10, "Duelist", 0.8)
{
}

Duelist::~Duelist()
{
	std::cout << "oof" << std::endl;
}

Rogue::Rogue() : Class(20, 8, "Rogue", 0.9)
{
}

Rogue::~Rogue()
{
	std::cout << "oof" << std::endl;
}