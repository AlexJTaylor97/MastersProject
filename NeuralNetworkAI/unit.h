#ifndef UNIT_H
#define UNIT_H


#include <SFML/Graphics.hpp>
#include "gameObject.h"
#include <iostream>
using namespace sf;
namespace order {
	enum orders { move, none };
}
class Unit: public GameObject
{
protected:
	float speed;		//unit speed
	float health;		//unit health
	float maxHealth;	//unti max health
	bool selected = false;	//bool if unit is selected
	bool canAttack = false;	//bool if unit can attack
	float attackSpeed;		//float of attack speed
	float attackReloadCounter = 0.0f;	//float to count own between attacks
	float attackRange;				//units attack range
	int attackDamage;				//unit attack damage
	std::vector<order::orders> orders;		//vector of orders 
	std::vector<sf::Vector2f> targetLocations;	//vecotr of orders locations
public:
	Unit();
	Unit(sf::Vector2f positionIn, sf::Vector2f halfExtentsIn, int healthIn,float speedIn, float attackSpeedIn,int attackDamageIn, float attackRangeIn, team::team teamIn, objectType::objectType typeIn);		//constructor
	~Unit();

	//default placeholders actual drawing and updated to be done by indivudal unit types code
	virtual void draw(RenderTarget &target, RenderStates states)const {};
	
	void update(float dt);							//update function
	void updatePosition(float dt);					//updates position
	void commandMove(sf::Vector2f targetLocation);			//commands the unit to move
	void commandChainMove(sf::Vector2f targetLocation);		//commands the unit to mve multiple times
	void stop();											//stops the unit moving
	int getHealth() { return health; }						//returns the units health

	void select() { selected = true; }			//selects the unts
	void deSelect() { selected = false; }		//de selects the unit
	bool getSelected() { return selected; }		//returns if it is selected
	bool getCanAttack() { return canAttack; }	//checks if the unti can attack
	int getDamage() { attackReloadCounter = 0.0f; return attackDamage; } //only called when attacking so this funciton also resets the attack cooldown
	float getAtackRange() { return attackRange; }			//returns attack range

	void damage(int damageIn) { health = health - damageIn; };		//damages the unit
};

#endif