/**
@file main.cpp
*/
#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>
#include "game.h"
using namespace sf;

void main()
{
	srand(time(NULL));
	RenderWindow window(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Strategy game with AI",sf::Style::Default);
	Game game(sf::Vector2i(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
	Clock clock;  //clock used to get dt
	Event event;
	bool playing = true;
	//sets to size of our view of the game
	sf::View gameView((sf::FloatRect(0, 2500, 800, 450)));

	//set to size of game world so that ui keeps size 
	sf::View uiView(sf::FloatRect(0, 0, 3000, 3000));

	window.setView(gameView);

	// game loop
	while (window.isOpen())
	{
		if (playing) {
			float dt = clock.getElapsedTime().asSeconds();
			clock.restart();

			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
			sf::Vector2f mouseUiRelativePosition = window.mapPixelToCoords(mousePosition, uiView);
			sf::Vector2f mouseWorldRelativePosition = window.mapPixelToCoords(mousePosition, gameView);

			//event handling
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed) {
					window.close();
				}
				if (event.type == Event::KeyPressed) {
					if (event.key.code == Keyboard::Key::Escape) {
						window.close();
					}
					else if (event.key.code == Keyboard::Key::Up && gameView.getCenter().y > 250) {
						gameView.move(0, -50);
						window.setView(gameView);
					}
					else if (event.key.code == Keyboard::Key::Down && gameView.getCenter().y < 2750) {
						gameView.move(0, 50);
						window.setView(gameView);
					}
					else if (event.key.code == Keyboard::Key::Left && gameView.getCenter().x > 250) {
						gameView.move(-50, 0);
						window.setView(gameView);
					}
					else if (event.key.code == Keyboard::Key::Right && gameView.getCenter().x < 2750) {
						gameView.move(50, 0);
						window.setView(gameView);
					}
					else {
						game.processKeyPress(event.key.code);
					}
				}

				if (event.type == Event::KeyReleased) {
					game.processKeyRelease(event.key.code);
				}
				if (event.type == Event::MouseButtonPressed) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						game.processMouseClick(mouseWorldRelativePosition, mouseUiRelativePosition);
					}
					if (event.mouseButton.button == sf::Mouse::Right) {
						game.processMouseRightClick(mouseWorldRelativePosition, mouseUiRelativePosition);
					}
				}
				if (event.type == Event::MouseButtonReleased) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						game.processMouseRelease(mouseUiRelativePosition);
					}
					if (event.mouseButton.button == sf::Mouse::Right) {
						game.processMouseRightRelease(mouseUiRelativePosition);
					}
				}
				if (event.type == Event::MouseWheelMoved) {
					// zoom in or out if its not excedeing its max or minimum
					if (event.mouseWheel.delta == 1 && gameView.getSize().x > 800) {
						gameView.setSize(gameView.getSize().x - 220, gameView.getSize().y - 123.75);
					}
					if (event.mouseWheel.delta == -1 && gameView.getSize().x < 3000) {
						gameView.setSize(gameView.getSize().x + 220, gameView.getSize().y + 123.75);
					}
				}
			}
			//if the mouse is at the edge of the screen move the screen
			if (mousePosition.x < 20 && gameView.getCenter().x > 250) {
				gameView.move(-2, 0);
			}
			if (mousePosition.x > VideoMode::getDesktopMode().width - 20 && gameView.getCenter().x < 2750) {
				gameView.move(2, 0);
			}
			if (mousePosition.y < 20 && gameView.getCenter().y > 250) {
				gameView.move(0, -2);
			}
			if (mousePosition.y > VideoMode::getDesktopMode().height - 50 && gameView.getCenter().y < 2750) {
				gameView.move(0, 2);
			}

			//updates the game
			game.update(dt, mouseWorldRelativePosition);

			window.clear(Color::Black);
			//switch to ui view to draw ui elements then back again
			game.draw(window, sf::RenderStates::RenderStates());

			window.setView(uiView);
			game.drawUi(window, sf::RenderStates::RenderStates());

			window.setView(gameView);

			window.display();

			//checks if the game is over
			if (game.computerWon || game.playerWon) {
				playing = false;		//if not keep playing
			}
		} else {
		//see if the user wants to play again
		window.close();
		system("cls");
		std::cout << "Game over" << std::endl;
		if (game.playerWon) {
			std::cout << "You won" << std::endl;
		}
		if (game.computerWon) {
			std::cout << "You lost" << std::endl << std::endl;
		}
		std::cout << "Would you like to play again? Y/N" << std::endl;
		char reply;
		std::cin >> reply;

		if (reply == 'y' || reply == 'Y') {
			//if so reset the game if not the game closes
			game = Game(sf::Vector2i(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height));
			gameView.setCenter(0, 2500);
			playing = true;
			window.create(VideoMode(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height), "Strategy game with AI", sf::Style::Default);
			game.playerWon = false;
			game.computerWon = false;
		} 
		}
	}	
}

