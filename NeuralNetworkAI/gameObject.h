#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Component.h"
#include "string"
#include "graphicsComponent.h"
#include "movementComponent.h"
#include "selectableComponent.h"
#include "spawnerComponent.h"
#include "buildRadiusComponent.h"
#include "resourceProducerComponent.h"
#include "transformComponent.h"
#include "combatComponent.h"
#include <unordered_map>
#include <typeindex>
namespace team {
	enum team { player, computer };
}


class GameObject
{
public:
	template <typename T>
	T* getComponent()
	{

		auto iter = m_components.find(typeid(T));

		if (iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			return dynamic_cast<T*>(iter->second);
		}

		// return null if we don't have a component of that type
		return nullptr;

	}

	template <typename T>
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		m_components[typeid(T)] = comp;
	}
	GameObject() {

	}
	GameObject(team::team teamIn, objectType::objectType typeIn) {
		team = teamIn;
		type = typeIn;

	}
	virtual void update(float dt) = 0;
	virtual void message(const std::string m) = 0;
	virtual std::string getMessage() = 0;
	objectType::objectType getType() { return type; }
	team::team getTeam() { return team; }
	void setType(objectType::objectType objectTypeIn) { type = objectTypeIn; }
	void setTeam(team::team teamIn) { team = teamIn; }

	float getAngleToTarget(sf::Vector2f targetPositionIn, sf::Vector2f positionIn) {
		//get difference in positions
		float dX, dY;
		dX = targetPositionIn.x - positionIn.x;
		dY = targetPositionIn.y - positionIn.y;

		//get distence between us and the ideal location
		float distance = sqrt(pow(dX, 2) + pow(dY, 2));

		//get angle to ideal location
		float angleOfIncident;
		angleOfIncident = (acos(dX / distance));
		angleOfIncident = angleOfIncident * (180 / 3.14);

		float angleToLocation;
		if (positionIn.y > targetPositionIn.y) {
			angleToLocation = 360 - angleOfIncident;
		}
		else {
			angleToLocation = angleOfIncident;
		}
		return angleToLocation;
	}

private:

	std::unordered_map<std::type_index, Component*> m_components;
	objectType::objectType type;
	team::team team;
};
#endif
