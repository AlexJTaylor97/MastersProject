
#include <SFML/Graphics.hpp>
#include "resource.h"
using namespace sf;


Resource::Resource() {

}
Resource::Resource(int xIn , int yIn) {
	position.x = xIn;
	position.y = yIn;
	sprite.setRadius(40);
	sprite.setFillColor(sf::Color(245, 185, 65, 255));
	sprite.setOrigin(40, 40);
	sprite.setPosition(position.x, position.y);

}

void Resource::draw(RenderTarget &target, RenderStates states)const {
	target.draw(sprite);
}


