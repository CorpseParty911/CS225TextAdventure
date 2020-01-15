#include "RPG Game.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

Item* nothing = (Item*)new Item("", 0, 0, 0);
Enemy* nobody = (Enemy*)new Enemy("", 0, 0, 0.0);
Enemy* Room::ENEMY_LIST[10] = { nobody,(Enemy*)new Enemy("Vampire Spawn",15,10,0.5),(Enemy*)new Enemy("Vampire",20,10,0.7),(Enemy*)new Enemy("Troll",45,10,0.65),(Enemy*)new Enemy("Vampire Knight",35,15,0.75),(Enemy*)new Enemy("Grue",90,25,0.6),(Enemy*)new Enemy("Skeleton",30,15,0.65),(Enemy*)new Enemy("Troll Elite",80,30,0.7),(Enemy*)new Enemy("Skeleton Knight",80,20,0.7),(Enemy*)new Enemy("Gaster",200,40,0.8) };
Item* Room::ITEM_LIST[20] = { nothing,(Item*)new Item("Dagger",5,0,0),(Item*)new Item("Shortsword",8,0,0),(Item*)new Item("Longsword",10,0,0),(Item*)new Item("Knife",13,0,0),(Item*)new Item("Glaive",15,0,0),(Item*)new Item("Greatsword",18,0,0),(Item*)new Item("Balmung",25,0,0),(Item*)new Item("Demonsbane",40,0,0),(Item*)new Item("Deathscythe",90,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("",0,0,0),(Item*)new Item("Key",0,0,0),(Item*)new Item("Candelabra",0,0,0) };

Room* emptyRoom = new Room();
Room** theMap;

Room::Room(int roomNumber, int* drops, int* encounters, Room** exits, string words, string roomName)
{
	if (drops != NULL)                //Only checking one because the default case is all three are NULL. Otherwise, they aren't NULL
		for (int i = 0; i < 10; i++)
		{
			items[i] = ITEM_LIST[drops[i]];
			encounter[i] = ENEMY_LIST[encounters[i]];
			connections[i] = exits[i];
		}
	name = roomName;
	text = words;
	number = roomNumber;
}

Item* Room::getItem(int index, Room* here)
{
	if (index < 0 || index > 9)  //Bounds check
		return nothing;
	return here->items[index];
}

Enemy* Room::getEncounter(int index, Room* here)
{
	if (index < 0 || index > 9)  //Bounds check
		return nobody;
	return here->encounter[index];
}

void Room::setEncounter(int index, Enemy* newEnemy)
{
	if (index < 0 || index > 9)  //Bounds check
		encounter[index] = newEnemy;
}

Room** Room::getConnections()
{
	return connections;
}

void Room::setConnections(int direction, Room* connection)
{
	if (direction < 0 || direction > 9)  //Bounds check
		return;
	connections[direction] = connection;
}

string Room::getText()
{
	return text;
}

string Room::getName()
{
	return name;
}

int Room::getNumber()
{
	return number;
}

void Room::gainItem(Item* item)
{
	for (int i = 0; i < 10; i++)  //Find an empty spot
		if (items[i]->getName() == "")
		{
			items[i] = item;
			break;
		}
}

Item* Room::loseItem(string itemName)
{
	for (int i = 0; i < 10; i++)
		if (items[i]->getName() == itemName)
		{
			Item* removed = items[i];
			items[i] = nothing;
			return removed;
		}
	return nothing;  //If it didn't find it
}

Room* findRoom(int roomIndex, int numRooms)
{
	for (int i = 0; i < numRooms; i++)
	{
		if (theMap[i] != emptyRoom)
			if (theMap[i]->getNumber() == roomIndex)
				return theMap[i];
	}
	return emptyRoom;  //If it didn't find it
}

Room* createMap(int numRooms)
{
	theMap = new Room*[numRooms];
	for (int i = 0; i < numRooms; i++)
	{
		theMap[i] = emptyRoom;  //Give them all a value for the time being
	}
	ifstream File;
	string text;
	string text2;
	string name;
	string line1;
	string line2;
	string line3;
	int encounters[10];
	int itemList[10];
	Room* exits[10];
	File.open("Map_Data.txt");
	if (File.is_open()) 
	{
		for (int i = 0; i < numRooms; i++)
		{
			getline(File, name);
			getline(File, text);
			getline(File, text2);
			getline(File, line1);
			getline(File, line2);
			getline(File, line3);
			for (int i = 0; i < 10; i++) 
			{
				size_t pos1 = line1.find(' ');  //String parsing into the respective arrays
				string enemyNumber = line1.substr(0, pos1);
				line1.erase(0, pos1 + 1);
				encounters[i] = atoi(enemyNumber.c_str());

				size_t pos2 = line2.find(' ');
				string itemNumber = line2.substr(0, pos2);
				line2.erase(0, pos2 + 1);
				itemList[i] = atoi(itemNumber.c_str());

				size_t pos3 = line3.find(' ');
				string roomNumber = line3.substr(0, pos3);
				line3.erase(0, pos3 + 1);
				exits[i] = findRoom(atoi(roomNumber.c_str()), numRooms);
			}
			Room* x = new Room(i + 1, itemList, encounters, exits, text + "\n" + text2, name);
			for (int i = 0; i < 10; i++) 
			{
				if (x->getConnections()[i] != emptyRoom)  //Make sure movement between the two rooms is possible
					x->getConnections()[i]->setConnections((i + 5) % 10, x);
			}
			theMap[i] = x;
			if (x->getName() == "Failure" || x->getName() == "")
			{
				throw("Invalid Name");  //If any room has either no name or "Failure", throw incomplete data exception
			}
		}
		File.close();
		return theMap[0];
	}
	else
	{
		throw("Invalid File");  //File didn't open
	}
}

int countLines()
{
	int numLines = 0;
	ifstream File;
	string line;
	File.open("Map_Data.txt");
	if (File.is_open()) 
	{
		while (File.peek() != EOF) //Next line isn't the end of the file ie not at the end of the last line
		{
			getline(File, line);  //Used to cycle through each line
			numLines++;
		}
		File.close();
	}
	else
	{
		throw("Invalid File");  //File didn't open
	}
	return numLines;
}