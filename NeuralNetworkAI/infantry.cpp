#include "infantry.h"

Infantry::Infantry(sf::Vector2f positionIn, team::team teamIn) :GameObject(teamIn, objectType::infantry) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(10, 10), positionIn, sf::Color::Magenta));
	}
	if (teamIn == team::player) {
		addComponent(new GraphicsComponent(sf::Vector2f(10, 10), positionIn, sf::Color::Cyan));
		addComponent(new SelectableComponent(sf::Vector2f(10, 10), positionIn));
	}
	addComponent(new TransformComponent(positionIn));
	addComponent(new MovementComponent(infantrySpeed));
	addComponent(new CombatComponent(infantryDamage, infantryRange,infantryHealth,infantryAttackSpeed, sf::Vector2f(10, 10),positionIn,true));
}

Infantry::~Infantry() {

}


void Infantry::update(float dt){
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
	std::cout << transform->getPosition().x << std::endl;



	getComponent<SelectableComponent>()->updatePosition(transform->getPosition());
	getComponent<CombatComponent>()->updatePosition(transform->getPosition());
	getComponent<GraphicsComponent>()->updatePosition(transform->getPosition());
}


void Infantry::message(const std::string msg) {

}