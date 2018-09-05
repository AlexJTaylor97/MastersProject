#include "refinery.h"

Refinery::Refinery(sf::Vector2f positionIn, team::team teamIn):GameObject(teamIn, objectType::refinery) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(50, 100), positionIn, sf::Color::Red));
	}
	if (teamIn == team::player) {
		addComponent(new GraphicsComponent(sf::Vector2f(50, 100), positionIn, sf::Color::Blue));
		addComponent(new SelectableComponent(sf::Vector2f(100, 100), positionIn));
	}
	
	addComponent(new TransformComponent(positionIn));
	addComponent(new ResourceProducerComponent(5.0f));
	addComponent(new CombatComponent(0, 0, refineryHealth, 0, sf::Vector2f(50, 100), positionIn, false));
}

Refinery::~Refinery() {

}


void Refinery::update(float dt) {
	ResourceProducerComponent* resource = getComponent<ResourceProducerComponent>();
	resource->update(dt);
}


void Refinery::message(const std::string msg) {

}