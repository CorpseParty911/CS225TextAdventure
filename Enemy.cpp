#include "RPG Game.h"
#include <iostream>
#include <string>
using namespace std;

Enemy::Enemy(string name, int health, int power, double acc)
{
	enemyName = name;
	this->health = health;
	maxHealth = health;
	damage = power;
	accuracy = acc;
}

void Enemy::regainHealth(int healing)
{
	health += healing;
	if (health > maxHealth)  //Don't go over max health
		health = maxHealth;
}

void Enemy::takeDamage(int hurt)
{
	health -= hurt;
}

int Enemy::getHealth()
{
	return health;
}

int Enemy::getDamage()
{
	return damage;
}

double Enemy::getAccuracy()
{
	return accuracy;
}

string Enemy::getName()
{
	return enemyName;
}

void Enemy::restore()
{
	health = maxHealth;
}