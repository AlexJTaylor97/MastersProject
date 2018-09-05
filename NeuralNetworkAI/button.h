#ifndef BUTTON_H
#define BUTTON_H


#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace sf;


class Button: public sf::Drawable
{
private:
	sf::Vector2f position;			//position of button
	sf::Vector2f halfExtents;		//half extents of button
	sf::RectangleShape sprite;		//button graphic
	sf::Text displayText;			//text to display
	sf::Text queueSizeText;			//size of queue (if used)
	sf::Font font;					//font used in button's text
	String text;					//string used in button's text
	bool show;						//bool to siwthc between displaying or hiding the button
	bool showNumber;				//bool to swithc between displaying or hiding a number such as queue (if used)
public:
	Button();
	Button(sf::Vector2f positionIn, String textIn,bool showNumberIn, sf::Vector2i screenSize);		//constructor which takes in its position and size and if it has a number to display
	~Button();

	void draw(RenderTarget &target, RenderStates states)const;			//draws the button						
	void updateAmount(int amountIn);							//updates the number or numebers displayed (if used)
	void updateAmount(int costIn, int amountIn);				//
	bool isClicked(sf::Vector2f mousePosition);					//bool to check if the button is clicked and takes in mouse position as argument
};

#endif
