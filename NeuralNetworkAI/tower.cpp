#include "tower.h"

Tower::Tower(sf::Vector2f positionIn, team::team teamIn) :GameObject(teamIn, objectType::tower) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(40, 40), positionIn, sf::Color::Red));
	}
	if (teamIn == team::player) {
		addComponent(new GraphicsComponent(sf::Vector2f(40, 40), positionIn, sf::Color::Blue));
		addComponent(new SelectableComponent(sf::Vector2f(40, 40), positionIn));
	}
	addComponent(new TransformComponent(positionIn));
	addComponent(new CombatComponent(towerDamage, towerRange, towerHealth, towerAttackSpeed, sf::Vector2f(40, 40), positionIn, true));
}

Tower::~Tower() {

}


void Tower::update(float dt) {

}


void Tower::message(const std::string msg) {

}