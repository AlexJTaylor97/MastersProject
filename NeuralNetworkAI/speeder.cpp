#include "speeder.h"

Speeder::Speeder(sf::Vector2f positionIn, team::team teamIn) :GameObject(teamIn, objectType::speeder) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(20, 20), positionIn, sf::Color::Magenta));
	}
	if (teamIn == team::player) {
		addComponent(new SelectableComponent(sf::Vector2f(20, 20), positionIn));
		addComponent(new GraphicsComponent(sf::Vector2f(20, 20), positionIn, sf::Color::Cyan));
	}
	addComponent(new TransformComponent(positionIn));
	addComponent(new MovementComponent(speederSpeed));
	addComponent(new CombatComponent(speederDamage, speederRange, speederHealth, speederAttackSpeed, sf::Vector2f(20, 20), positionIn, true));
}

Speeder::~Speeder() {

}


void Speeder::update(float dt) {
	TransformComponent* transform = getComponent<TransformComponent>();

	MovementComponent* movement = getComponent<MovementComponent>();

	if (movement->getOrder() == orderTypes::move) {
		float dX, dY;
		dX = transform->getPosition().x - movement->getTargetPosition().x;
		dY = transform->getPosition().y - movement->getTargetPosition().y;

		//get distence between us and the ideal location
		float distance = sqrt(pow(dX, 2) + pow(dY, 2));

		if (distance > 5) {
			transform->setPosition(movement->updatePosition(dt, transform->getPosition()));
		}
		else {
			movement->stop();
		}

	}




	getComponent<SelectableComponent>()->updatePosition(transform->getPosition());
	getComponent<CombatComponent>()->updatePosition(transform->getPosition());
	getComponent<GraphicsComponent>()->updatePosition(transform->getPosition());
}


void Speeder::message(const std::string msg) {

}