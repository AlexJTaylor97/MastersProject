#include "barracks.h"

Barracks::Barracks(sf::Vector2f positionIn, team::team teamIn) :GameObject(teamIn, objectType::barracks) {
	if (teamIn == team::computer) {
		addComponent(new GraphicsComponent(sf::Vector2f(100, 50), positionIn, sf::Color::Red));
	}
	if (teamIn == team::player) {
		addComponent(new GraphicsComponent(sf::Vector2f(100, 50), positionIn, sf::Color::Blue));
		addComponent(new SelectableComponent(sf::Vector2f(100, 100), positionIn));
	}
	addComponent(new TransformComponent(positionIn));
	addComponent(new SpawnerComponent(positionIn));
	addComponent(new CombatComponent(0, 0, barracksHealth, 0, sf::Vector2f(50, 100), positionIn, false));

}

Barracks::~Barracks() {

}


void Barracks::update(float dt) {
	SpawnerComponent* spawner = getComponent<SpawnerComponent>();
	spawner->update(dt);
}


void Barracks::message(const std::string msg) {

}