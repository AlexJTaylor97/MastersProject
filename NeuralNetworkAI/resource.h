#ifndef RESOURCE_H
#define RESOURCE_H

#include <SFML/Graphics.hpp>
#include "gameObject.h"
using namespace sf;

class Resource: public sf::Drawable
{
private:
	sf::Vector2f position;		//position of resource
	CircleShape sprite;			//sprite of resource
	bool claimed = false;		//if the resource is claimed or not
public:
	Resource();
	Resource(int xIn, int yIn);			//constructor
	void draw(RenderTarget &target, RenderStates states)const;		//draw function

	void claimResource() { claimed = true; }			//claims the resouce
	void unClaimResource() { claimed = false; }			//unclaims the resource
	bool isClaimed() { return claimed; }				//checks if resource is claimed
	sf::Vector2f getPosition() { return position; }		//returns position of resource


};


#endif