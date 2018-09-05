#include "outpost.h"

Outpost::Outpost(sf::Vector2f positionIn, team::team teamIn) :GameObject(teamIn, objectType::outpost) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(60, 60), positionIn, sf::Color::Red));
	}
	if (teamIn == team::player) {
		addComponent(new GraphicsComponent(sf::Vector2f(60, 60), positionIn, sf::Color::Blue));
		addComponent(new SelectableComponent(sf::Vector2f(60, 60), positionIn));
	}
	addComponent(new TransformComponent(positionIn));
	addComponent(new BuildRadiusComponent(outpostBuildRadius, positionIn));
	addComponent(new CombatComponent(0, 0, outpostHealth, 0, sf::Vector2f(50, 100), positionIn, false));
}

Outpost::~Outpost() {

}


void Outpost::update(float dt) {

}


void Outpost::message(const std::string msg) {

}