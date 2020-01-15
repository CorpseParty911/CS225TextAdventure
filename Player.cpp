#include "RPG Game.h"
#include <iostream>
#include <string>

extern Item* nothing;

Player::Player(std::string name, std::string classChoice)
{
	if (classChoice == "Barbarian")  //Based on user choice
		playerClass = new Barbarian();
	if (classChoice == "Fighter")
		playerClass = new Fighter();
	if (classChoice == "Duelist")
		playerClass = new Duelist();
	if (classChoice == "Rogue")
		playerClass = new Rogue();
	this->name = name;
	for (int i = 0; i < 10; i++)
	{
		inventory[i] = nothing;  //Nobody starts with items these days
	}
}

std::string Player::getName()
{
	return name;
}

Class* Player::getClass()
{
	return playerClass;
}

Item* Player::getItem(int index)
{
	return inventory[index];
}

void Player::pickUp(Item* item)
{
	for (int i = 0; i < 10; i++)
		if (inventory[i]->getName() == "")  //Find an open spot
		{
			inventory[i] = item;
			break;
		}
}

Item* Player::loseItem(std::string itemName)
{
	for (int i = 0; i < 10; i++)
		if (inventory[i]->getName() == itemName)
		{
			Item* removed = inventory[i];
			inventory[i] = (Item*)new Item("", 0, 0, 0);
			return removed;
		}
	return nothing;  //If didn't find the item
}

Player::~Player()
{
	std::cout << "Unfortunately, you have died." << std::endl;  //Doesn't actually do much, but oh well
}