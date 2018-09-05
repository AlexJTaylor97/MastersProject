#include "base.h"

Base::Base(sf::Vector2f positionIn, team::team teamIn) :GameObject(teamIn, objectType::base) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(100, 100), positionIn, sf::Color::Red));
	}
	if (teamIn == team::player) {
		addComponent(new GraphicsComponent(sf::Vector2f(100, 100), positionIn, sf::Color::Blue));
		addComponent(new SelectableComponent(sf::Vector2f(100, 100), positionIn));
	}
	addComponent(new TransformComponent(positionIn));
	addComponent(new BuildRadiusComponent(baseBuildRadius, positionIn));
	addComponent(new ResourceProducerComponent(10.0f));
	addComponent(new CombatComponent(0, 0, baseHealth, 0, sf::Vector2f(50, 100), positionIn, false));
}

Base::~Base() {

}


void Base::update(float dt) {
	ResourceProducerComponent* resource = getComponent<ResourceProducerComponent>();
	resource->update(dt);
}


void Base::message(const std::string msg) {

}