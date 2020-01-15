#include "RPG Game.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include <string>
#include <random>
using namespace std;

void playGame(sf::RenderWindow&, sf::RectangleShape&, sf::RectangleShape&, sf::RectangleShape&, sf::Music&, sf::Music&, sf::Text&, sf::Text&, sf::Text&, sf::Text&, sf::Text&, sf::Text&, Player*, Room*);
string operator<<(sf::RenderWindow &window, Room &current);
void updateInventory(Player* p, sf::Text&);

extern Room* emptyRoom;
extern Room** theMap;
extern Item* nothing;
extern Enemy* nobody;

int main()
{
	try
	{
		sf::Font comicSans;
		sf::Text text("", comicSans, 30);
		sf::Text moreText("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWhat would you like to do? ", comicSans, 30);
		sf::Text responseText("", comicSans, 30);
		sf::Text playerText("", comicSans, 30);
		sf::Text fightText("", comicSans, 30);
		sf::Text inventoryText("", comicSans, 30);
		sf::Texture texture;
		sf::RectangleShape rectangle(sf::Vector2f(1920.f, 1080.f));  //A lot of variable declaration...
		sf::Texture texture2;
		sf::RectangleShape rectangle2(sf::Vector2f(1920.f, 370.f));
		sf::Texture texture3;
		sf::RectangleShape rectangle3(sf::Vector2f(1920.f, 770.f));
		sf::Music sabaton;
		sf::Music sans;
		//sf::Font wingdings; I wish this would work
		string words = "Enter your name: ";
		sf::RenderWindow window(sf::VideoMode(1920, 1080), "Derp Quest", sf::Style::Fullscreen); //Initialize the window
		window.clear(sf::Color::Black);
		window.display();
		window.requestFocus();
		if (!comicSans.loadFromFile("ComicSans.ttf"))
		{
			cout << "Failed to load font."; //Couldn't load the font file.
		}
		else if (!texture.loadFromFile("texture.jpg") || !texture2.loadFromFile("texture1.jpg") || !texture3.loadFromFile("gaster.jpg"))
		{
			cout << "Failed to load texture."; //Couldn't load one or more images.
		}
		else if (!sabaton.openFromFile("toHellAndBack.ogg") || !sans.openFromFile("Megalovania.ogg"))
		{
			cout << "Failed to load sound."; //Couldn't load one or both sound files.
		}
		else //Either way, exit if the font, texture, or sound doesn't load
		{
			rectangle.setTexture(&texture);
			rectangle.setTextureRect(sf::IntRect(0, 0, 1920, 1080));
			rectangle.setPosition(sf::Vector2f(0.f, 0.f));
			rectangle2.setTexture(&texture2);
			rectangle2.setTextureRect(sf::IntRect(0, 310, 1920, 370));  //Placement of stuff
			rectangle2.setPosition(sf::Vector2f(0.f, 710.f));
			rectangle3.setTexture(&texture3);
			rectangle3.setTextureRect(sf::IntRect(0, 310, 1920, 770));
			rectangle3.setPosition(sf::Vector2f(0.f, 0.f));
			string name;
			string classChoice;
			sf::Event event;
			bool done = false;
			while (!done)  //Allow player to enter their name
			{
				window.clear(sf::Color::Black);
				text.setString(words);
				text.setFillColor(sf::Color::White);
				window.draw(text);
				while (window.pollEvent(event))
				{
					switch (event.type)
					{
						// window closed
					case sf::Event::Closed:
						window.close();
						break;

						// key pressed
					case sf::Event::KeyReleased:
						if (event.key.code == sf::Keyboard::Enter) //Done on enter
							done = true;
						else if (event.key.code == sf::Keyboard::BackSpace && words.length() > 17)
							words = words.substr(0, words.length() - 1); //Making backspace work
						break;
					case sf::Event::TextEntered:
						if (isalpha(event.text.unicode) || static_cast<char>(event.text.unicode) == ' ')
							words += static_cast<char>(event.text.unicode);
						break;
					}
				}
				window.display();
			}
			name = words.substr(17, words.length());

			while (classChoice != "Barbarian" && classChoice != "Fighter" && classChoice != "Duelist" && classChoice != "Rogue") {
				words = "Choose a class: Barbarian, Fighter, Duelist, or Rogue: ";
				sf::Event event;
				done = false;
				while (!done) //Wait till enter
				{
					window.clear(sf::Color::Black);
					text.setString(words);
					text.setFillColor(sf::Color::White);
					window.draw(text);
					while (window.pollEvent(event))
					{
						switch (event.type)
						{
							// window closed
						case sf::Event::Closed:
							window.close();
							break;

							// key pressed
						case sf::Event::KeyReleased:
							if (event.key.code == sf::Keyboard::Enter)
								done = true;
							else if (event.key.code == sf::Keyboard::BackSpace && words.length() > 55)
								words = words.substr(0, words.length() - 1);
							break;
						case sf::Event::TextEntered:
							if (isalpha(event.text.unicode) || static_cast<char>(event.text.unicode) == ' ')
								words += static_cast<char>(event.text.unicode);
							break;
						}
					}
					window.display();
				}
				classChoice = words.substr(55, words.length());
			}
			name[0] = toupper(name[0]); //Capitalize
			Player* p = new Player(name, classChoice); //Create player
			playerText.setString(p->getName() + "\nLevel " + to_string(p->getClass()->getLevel()) + " " + classChoice);
			playerText.setCharacterSize(45);
			playerText.setPosition(sf::Vector2f(0, 715.f)); //Formatting and position
			inventoryText.setCharacterSize(45);
			inventoryText.setPosition(sf::Vector2f(500.f, 715.f));
			try
			{
				int lines = countLines();
				Room* start = createMap(lines / 6); //Does exactly what it says
				playGame(window, rectangle, rectangle2, rectangle3, sabaton, sans, text, moreText, responseText, fightText, inventoryText, playerText, p, start);
				for (long long i = 0; i < 1000000000; i++); //Wait timer...
				return 0;
			}
			catch (const char* fail)
			{
				if (fail == "Invalid Name") //One of the rooms that was created was named "Failure" or had no name
				{
					words = "Map data incomplete.";
				}
				else if (fail == "Invalid File") // Couldn't open the file
				{
					words = "Data file could not be opened.";
				}
			}
		}
		window.clear(sf::Color::Black);
		words += "\nExiting now.";
		text.setString(words);
		text.setFillColor(sf::Color::White);
		window.draw(text);
		window.display();
		for (long long i = 0; i < 1000000000; i++); //Wait timer so the reason for exiting can be read
		window.close();
		return -1;
	}
	catch (...)
	{
		cout << "Caught an exception." << endl;
		for (long long i = 0; i < 1000000000; i++);
	}
}

void fight(sf::RenderWindow &window, sf::Text &fightText, Enemy* enemy, Player* p, Item* weapon)
{
	random_device rd;
	string words = "";

	if (weapon != nothing)
	{
		if (rd() <= (4294967295 * p->getClass()->getAccuracy()))  //Did I hit?
		{
			enemy->takeDamage(weapon->getAttack());
			if (enemy->getName() != "Gaster")
				words += "You successfully landed a solid hit on the " + enemy->getName() + ".";
			else
				words += "You successfully landed a solid hit on " + enemy->getName() + ".";
		}
		else  //No
		{
			if (enemy->getName() != "Gaster")
				words += "You tried to hit the " + enemy->getName() + "... and failed...";
			else
				words += "You tried to hit " + enemy->getName() + "... and failed...";
		}
	}
	if (enemy->getHealth() > 0)
	{
		if (rd() <= (4294967295 * enemy->getAccuracy()))  //Did they hit?
		{
			p->getClass()->takeDamage(enemy->getDamage());
			if (words != "")
				words += "\n";
			if (enemy->getName() != "Gaster")  //Because proper noun
				words += "The " + enemy->getName() + " hit you pretty hard.";
			else
				words += enemy->getName() + " hit you pretty hard.";
		}
		else  //No
		{
			if (words != "")
				words += "\n";
			if (enemy->getName() != "Gaster")  //Because proper noun
				words += "Fortunately for you the " + enemy->getName() + " missed you, albeit by a hair.";
			else
				words += "Fortunately for you " + enemy->getName() + " missed you, albeit by a hair.";
		}
	}
	words = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n" + words;
	fightText.setString(words);
}

void playGame(sf::RenderWindow &window, sf::RectangleShape &rectangle, sf::RectangleShape &rectangle2, sf::RectangleShape &rectangle3, sf::Music &sabaton, sf::Music &sans, sf::Text &text, sf::Text &moreText, sf::Text &responseText, sf::Text &fightText, sf::Text &inventoryText, sf::Text &playerText, Player* p, Room* current)
{
	random_device rd;
	updateInventory(p, inventoryText);
	string directions[10] = { "Up","North","Northeast","East","Southeast","Down","South","Southwest","West","Northwest" };
	int encounterIndex = rd() % 10;
	int directionIndex;
	bool roomChange = true;
	string decision = "";
	string previousDecision = "";
	string response = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; //Variable declaration...
	string subString;
	int prevDirection = 0;
	size_t pos;
	bool dark = true;
	bool bossSoundtrack = false;
	bool frontDoor = false;
	Enemy* currentEnemy = current->getEncounter(encounterIndex, current);
	int enemiesKilled = 0;

	sabaton.play();
	sabaton.setLoop(true); //Start the music
	sabaton.setVolume(10.f);
	text.setFillColor(sf::Color::Red);
	moreText.setFillColor(sf::Color::Red);
	responseText.setFillColor(sf::Color::Red);
	playerText.setFillColor(sf::Color::Red);
	inventoryText.setFillColor(sf::Color::Red);  //More formatting
	fightText.setFillColor(sf::Color::Red);
	window.clear(sf::Color::Black);
	while (window.isOpen() && p->getClass()->getHealth() > 0)
	{
		if (current->getNumber() == 146 && !bossSoundtrack) //Start boss music
		{
			sabaton.stop();
			sans.play();
			sans.setLoop(true);
			sans.setVolume(10.f);
			bossSoundtrack = true;
		}
		else if (current->getNumber() != 146 && bossSoundtrack) //Stop boss music
		{
			sans.stop();
			sabaton.play();
			sabaton.setLoop(true);
			sabaton.setVolume(10.f);
			bossSoundtrack = false;
		}
		sf::Event event;
		dark = true;
		if (current->getNumber() >= 28 && current->getNumber() <= 91) //If in the caves
		{
			for (int i = 0; i < 10; i++) {
				if (p->getItem(i)->getName() == "Candelabra" || current->getItem(i, current)->getName() == "Candelabra") //If there is light.
				{
					dark = false; //It is apparently not dark in here
					break;
				}
			}
		}
		else //It is apparently not dark in here
		{
			dark = false;
		}

		if (!dark && roomChange)
		{
			currentEnemy = current->getEncounter(encounterIndex, current);  //Generate an encounter
			window.clear(sf::Color::Black);
			text.setString(window << *current);  //Operator overload returns necessary information
			if (currentEnemy->getName() == "Gaster")  //Because proper noun
			{
				text.setString(text.getString() + "\nGaster is here.");
			}
			else if (currentEnemy->getName() != "")  //Enemy exists
			{
				text.setString(text.getString() + "\nThere is a " + currentEnemy->getName() + " here.");
			}
			roomChange = false;  //Don't bother with this every time
		}
		else if (dark)
		{
			window.clear(sf::Color::Black);
			text.setString("It is dark in here. You are likely to be eaten by a grue.");  //Hmmmmmmm...
		}

		// while there are pending events...
		while (window.pollEvent(event))
		{
			// check the type of the event...
			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				window.close();  //Closes window. Though it's fullscreen, so that's irrelevant
				break;

				// key pressed
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Enter)
				{
					previousDecision = decision;  //Press up for last decision
					fightText.setString("");
					responseText.setString("");
					window.clear(sf::Color::Black);
					pos = decision.find(' ');
					subString = decision.substr(0, pos);  //String parsing
					decision.erase(0, pos + 1);
					subString[0] = toupper(subString[0]);

					if (subString == "Quit")
					{
						moreText.setString("");   //Self Explanatory
						responseText.setString(response + "Alright. Baby.");
						p->getClass()->takeDamage(p->getClass()->getHealth());
					}
					else if (subString == "")
						responseText.setString(response + "Say something, please."); //If nothing entered
					else
					{
						for (directionIndex = 0; directionIndex < 10; directionIndex++)
						{
							if (directions[directionIndex] == subString)
							{
								break;  //A direction was chosen
							}
						}
						if (directionIndex != 10)  //Move somewhere
						{
							if (current->getNumber() >= 28 && current->getNumber() <= 91 && dark)
							{
								if (directionIndex == (prevDirection + 5) % 10) //Moving, as long as it's backwards
								{
									encounterIndex = rd() % 10;
									current = current->getConnections()[directionIndex];
									prevDirection = directionIndex;
									roomChange = true;
								}
								else
								{
									responseText.setString(response + "You walked into the fangs of a slavering grue."); //If you move in a dark room, you die
									p->getClass()->takeDamage(p->getClass()->getHealth());
								}
							}
							else if (current->getConnections()[directionIndex] == emptyRoom)  //Nah fam
							{
								responseText.setString(response + "You can't go that way.");
							}
							else if (currentEnemy->getName() != "")
							{
								if (directionIndex == (prevDirection + 5) % 10) //Moving places, provided nothing is blocking the path
								{
									encounterIndex = rd() % 10;
									current = current->getConnections()[directionIndex];
									prevDirection = directionIndex;
									roomChange = true;
								}
								else
									responseText.setString(response + "The " + currentEnemy->getName() + " blocks the path.");
							}
							else if (current->getNumber() == 14 && subString == "West" && !frontDoor)  //The door may be locked
							{
								for (int i = 0; i < 10; i++)
								{
									if (p->getItem(i)->getName() == "Key")
									{
										responseText.setString(response + "You've unlocked the front door. Unfortunately, the key got stuck in the lock.");
										p->loseItem("Key");
										updateInventory(p, inventoryText);
										frontDoor = true;
									}
								}
								if (!frontDoor)  //Indeed it is
									responseText.setString(response + "The front door is locked.");
							}
							else
							{
								encounterIndex = rd() % 10;
								current = current->getConnections()[directionIndex]; //General move condition
								prevDirection = directionIndex;
								roomChange = true;
							}
						}
						else
						{
							if (current->getNumber() >= 28 && current->getNumber() <= 91 && dark)
								responseText.setString(response + "You can't see in here.");  //Most people can't see in the dark...
							else
							{
								if ("Kill" == subString) //Fight
								{
									pos = decision.find(' ');
									subString = decision.substr(0, pos);  //String parsing
									decision.erase(0, pos + 1);
									subString[0] = toupper(subString[0]);
									if (subString == p->getName() || subString == "Myself") //Essentially quit
									{
										responseText.setString(response + "The madness went to your head.");
										p->getClass()->takeDamage(p->getClass()->getHealth());
										moreText.setString("");
									}
									else
									{
										if (currentEnemy->getName().find(' ') != string::npos)
										{
											decision[0] = toupper(decision[0]);
											pos = decision.find(' ');
											subString += " " + decision.substr(0, pos);  //String parsing, though this one's because the enemy's name is two words
											decision.erase(0, pos + 1);
										}
										if (currentEnemy->getName() == "")
											responseText.setString(response + "I don't see any enemies here."); //Nothing to kill
										else if (currentEnemy->getName() == subString)
										{
											pos = decision.find(' ');
											subString = decision.substr(0, pos);
											decision.erase(0, pos + 1);  //String parsing
											subString[0] = toupper(subString[0]);
											decision[0] = toupper(decision[0]);
											if ("With" == subString)
											{
												bool found = false;
												for (int i = 0; i < 10; i++)
												{
													if (p->getItem(i)->getName() == decision)  //Have the item named
													{
														fight(window, fightText, currentEnemy, p, p->getItem(i));
														found = true;
														break;
													}
												}
												if (!found)  //You don't have the item
													responseText.setString(response + "Please tell me you have a weapon to use.");
											}
											else
											{  //Fight, but without a "With"
												responseText.setString(response + "Please tell me you're planning on using a weapon.");
											}
										}
										else
											responseText.setString(response + "I don't see any " + subString + " here.");
									}
									if (currentEnemy->getName() != "" && currentEnemy->getHealth() <= 0) //Is the enemy dead?
									{
										responseText.setString(response + "The " + currentEnemy->getName() + " falls to the ground, disappearing in a cloud of black smoke.");
										currentEnemy->restore();
										current->setEncounter(encounterIndex, nobody);
										roomChange = true;
										enemiesKilled++;
										if (enemiesKilled >= 5 * p->getClass()->getLevel()) //Level up condition
										{
											//enemiesKilled = 0;
											p->getClass()->levelUp();
											p->getClass()->regainHealth(p->getClass()->getMaxHealth());
											playerText.setString(p->getName() + "\nLevel " + to_string(p->getClass()->getLevel()) + " " + p->getClass()->getName());
										}
									}
								}
								else if ("Take" == subString) //Pick something up
								{
									pos = decision.find(' ');
									subString = decision.substr(0, pos);
									decision.erase(0, pos + 1);  //String parsing
									subString[0] = toupper(subString[0]);
									bool found = false;
									for (int i = 0; i < 10; i++)
									{
										if (current->getItem(i, current)->getName() == subString)
										{
											p->pickUp(current->loseItem(current->getItem(i, current)->getName()));
											if (currentEnemy->getName() != "")
												fight(window, fightText, currentEnemy, p, nothing); //If there's an enemy in the room
											found = true;
											roomChange = true;
											updateInventory(p, inventoryText);
											break;
										}
									}
									if (!found)
										responseText.setString(response + "I don't see any " + subString + " here.");
								}
								else if ("Drop" == subString) //Drop something being carried
								{
									pos = decision.find(' ');
									subString = decision.substr(0, pos);
									decision.erase(0, pos + 1);  //String parsing
									subString[0] = toupper(subString[0]);
									bool found = false;
									for (int i = 0; i < 10; i++)
										if (p->getItem(i)->getName() == subString)
										{
											current->gainItem(p->loseItem(p->getItem(i)->getName()));
											if (currentEnemy->getName() != "")
												fight(window, fightText, currentEnemy, p, nothing); //If there's an enemy in the room
											roomChange = true;
											found = true;
											updateInventory(p, inventoryText);
											break;
										}
									if (!found)
										responseText.setString(response + "You do not have a " + subString + ".");
								}
								else if ("Wait" == subString) //Restore a little health
								{
									if (p->getClass()->getHealth() < p->getClass()->getMaxHealth())
									{
										responseText.setString(response + "You diagnosed the stab wounds on yourself as stab wounds. You feel a little better now.");
										p->getClass()->regainHealth(2 * p->getClass()->getLevel());
										if (currentEnemy->getName() != "")
											fight(window, fightText, currentEnemy, p, nothing); //If there's an enemy in the room
									}
									else
										responseText.setString(response + "You have no wounds, don't seem to be bleeding anywhere, and you are wasting my time.");
								}
								else if ("Jump" == subString) //Legitimately no point...
								{
									if (current->getNumber() == 21) //Except in the kitchen
									{
										responseText.setString(response + "Geronimo.....\nAs you take your last breath, you feel relieved of your burdens.");
										moreText.setString("");
										p->getClass()->takeDamage(p->getClass()->getHealth());
									}
									else
										responseText.setString(response + "Wheeee!");
								}
								else//If nothing useful entered
									responseText.setString(response + "Sorry, I am lacking in reasonable intelligence parameters, and therefore cannot comprehend your statement.");
							}
						}
					}
					decision = "";
					if (p->getClass()->getHealth() > 0) //If you died, you died. If not, next prompt
						moreText.setString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWhat would you like to do? ");
					else
						moreText.setString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nUnfortunately, you have died.");
				}
				else if (event.key.code == sf::Keyboard::BackSpace && decision.length() > 0)
				{
					window.clear(sf::Color::Black);
					decision = decision.substr(0, decision.length() - 1); //Making backspace work
					moreText.setString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWhat would you like to do? " + decision);
				}
				break;

			case sf::Event::TextEntered:
				if (isalpha(event.text.unicode) || static_cast<char>(event.text.unicode) == ' ')
				{
					decision += static_cast<char>(event.text.unicode); //Entering text
					moreText.setString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWhat would you like to do? " + decision);
				}
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Up)
				{
					decision = previousDecision; //Select previous decision
					moreText.setString("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nWhat would you like to do? " + decision);
				}
				break;

				// we don't process other types of events
			default:
				break;
			}
		}
		if (current->getNumber() == 146)
			window.draw(rectangle3);
		else if (currentEnemy->getName() != "")
			window.draw(rectangle);
		window.draw(rectangle2);
		window.draw(text);
		window.draw(moreText);
		window.draw(responseText); //Draw everything
		window.draw(playerText);
		window.draw(inventoryText);
		window.draw(fightText);
		window.display();
	}
}

string operator<<(sf::RenderWindow &window, Room &room)
{
	string words = room.getName() + "\n" + room.getText();
	for (int i = 0; i < 10; i++)
	{
		if (room.getItem(i, &room)->getName() != "") //Obtain text for room
		{
			words += "\nThere is a " + room.getItem(i, &room)->getName() + " here.";
		}
	}
	return words;
}

void updateInventory(Player *p, sf::Text &inventoryText)
{
	inventoryText.setString("You have:");
	bool found = false;
	int itemCount = 0;
	for (int i = 0; i < 10; i++)
	{
		if (p->getItem(i)->getName() != "") //Set inventory text
		{
			itemCount++;
			found = true;
			inventoryText.setString(inventoryText.getString() + "   " + p->getItem(i)->getName());
			if (itemCount == 3)
			{
				itemCount = 0;
				inventoryText.setString(inventoryText.getString() + "\n             ");
			}
		}
	}
	if (!found) //Don't have anything
		inventoryText.setString(inventoryText.getString() + "   Nothing.");
}