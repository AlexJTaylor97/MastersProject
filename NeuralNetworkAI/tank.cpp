#include "tank.h"

Tank::Tank(sf::Vector2f positionIn, team::team teamIn) :GameObject(teamIn, objectType::tank) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(30, 30), positionIn, sf::Color::Magenta));
	}
	if (teamIn == team::player) {
		addComponent(new SelectableComponent(sf::Vector2f(30, 30), positionIn));
		addComponent(new GraphicsComponent(sf::Vector2f(30, 30), positionIn, sf::Color::Cyan));
	}
	addComponent(new TransformComponent(positionIn));

	addComponent(new MovementComponent(tankSpeed));
	addComponent(new CombatComponent(tankDamage, tankRange, tankHealth, tankAttackSpeed, sf::Vector2f(30, 30), positionIn, true));
}

Tank::~Tank() {

}


void Tank::update(float dt) {
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
		} else {
			movement->stop();
		}
		
	}

	

	
	getComponent<SelectableComponent>()->updatePosition(transform->getPosition());
	getComponent<CombatComponent>()->updatePosition(transform->getPosition());
	getComponent<GraphicsComponent>()->updatePosition(transform->getPosition());


	
}

void Tank::message(const std::string msg) {

}