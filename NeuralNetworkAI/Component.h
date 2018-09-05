#pragma once
#include <SFML/Graphics.hpp>
#include <string>
//enum for object type
namespace objectType {
	enum objectType { base, refinery, barracks, resource, infantry, speeder, tank, outpost, tower };
}

namespace orderTypes {
	enum orderTypes { move, none };
}

class Component
{
public:
	virtual void update(float dt) = 0;
	virtual void message(const std::string m) = 0;
};

