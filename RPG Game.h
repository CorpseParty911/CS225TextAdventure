#include <string>

#ifndef RPG_GAME_H
#define RPG_GAME_H

class Class
{
protected:
	int healthPerLevel;
	int level;
	int maxHealth;
	int health;
	double accuracy;
	std::string className;
public:
	Class(int, int, std::string, double);
	void levelUp();
	void regainHealth(int healing);
	void takeDamage(int hurt);
	int getLevel();
	int getHealth();
	int getMaxHealth();
	double getAccuracy();
	std::string getName();
	void display();
};

class Barbarian : public Class
{
public:
	Barbarian();
	~Barbarian();
};

class Duelist : public Class
{
public:
	Duelist();
	~Duelist();
};

class Fighter : public Class
{
public:
	Fighter();
	~Fighter();
};

class Rogue : public Class
{
public:
	Rogue();
	~Rogue();
};

class Item
{
private:
	std::string name;
	int attack;
	int defense;
	int weight;
public:
	Item();
	Item(std::string, int, int, int);
	std::string getName();
	int getAttack();
	int getDefense();
	int getWeight();
};

class Enemy
{
protected:
	int maxHealth;
	int health;
	int damage;
	double accuracy;
	std::string enemyName;
public:
	Enemy(std::string, int, int, double);
	void regainHealth(int healing);
	void takeDamage(int hurt);
	int getHealth();
	int getDamage();
	double getAccuracy();
	std::string getName();
	void restore();
};

class Player
{
private:
	Class* playerClass;
	Item* inventory[10];
	std::string name;
public:
	Player(std::string, std::string);
	virtual ~Player();
	std::string getName();
	Class* getClass();
	Item* getItem(int);
	void pickUp(Item*);
	Item* loseItem(std::string);
};

class Room
{
private:
	static Enemy* ENEMY_LIST[10];
	static Item* ITEM_LIST[20];
	Item* items[10];
	Enemy* encounter[10];
	Room* connections[10];
	std::string text;
	std::string name;
	int number;
public:
	Room(int = 0, int* = NULL, int* = NULL, Room** = NULL, std::string = "", std::string = "Failure");
	static Item* getItem(int, Room*);
	static Enemy* getEncounter(int, Room*);
	void setEncounter(int, Enemy*);
	Room** getConnections();
	void setConnections(int, Room*);
	std::string getText();
	std::string getName();
	int getNumber();
	void gainItem(Item*);
	Item* loseItem(std::string);
};

Room* findRoom(int, int);
Room* createMap(int);
int countLines();


#endif //RPG_GAME_H
