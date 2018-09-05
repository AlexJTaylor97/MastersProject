#pragma once
#include "Component.h"


class BuildRadiusComponent : public Component
{ 
private:
	sf::CircleShape buildRadiusSprite;			//base build radius
	float buildRadius;
public:
	BuildRadiusComponent() {}
	BuildRadiusComponent(float radiusIn, sf::Vector2f positionIn) {	buildRadius = radiusIn; 

		buildRadiusSprite.setFillColor(sf::Color(130, 60, 80, 125));
		buildRadiusSprite.setOrigin(750, 750);
		buildRadiusSprite.setRadius(750);
		buildRadiusSprite.setPosition(positionIn);
	}

	void update(float dt) override {}
	void message(const std::string m) override {}

	sf::CircleShape* getBuildRadiusSprite() { return &buildRadiusSprite; }

	float getBuildRadius() { return buildRadius; }
};