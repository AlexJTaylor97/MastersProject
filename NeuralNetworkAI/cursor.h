#ifndef CURSOR_H
#define CURSOR_H


#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

namespace building{
	enum building { bldRefinery, bldBarracks, bldTower, bldOutpost };
}

class Cursor: public sf::Drawable
{
private:
	sf::Vector2f position;								//position of cursor
	sf::RectangleShape buildingGhost;					//shape drawn when palcing a building to show its future location
	sf::ConvexShape selectionBox;						//the white box you see when you click and drag
	building::building selectedBuilding;				//type of building selected
	bool placingBuilding = false;						//booleans to check what the cursor is currently doing
	bool selectingUnits = false;
	float xSelectionBounds, ySelectionBounds;			//positions of the white selection boxes corners
	float xSelectionBounds2, ySelectionBounds2;			//
public:
	Cursor();
	~Cursor();

	void draw(RenderTarget &target, RenderStates states)const;		//draws either a building ghost or selection box
	void update(float dt, sf::Vector2f mousePosition);		//updates the cursors values

	void placeBuilding(sf::RectangleShape buildingIn);			//prompts the cursor to place a building
	void groupSelect(sf::Vector2f mouseLocation);				//prompts the curosor to select groups 

	void stopPlacingBuilding() { placingBuilding = false; };		//prompts the cursor to stop placing buildings 
	void stopSelectingUnits() { selectingUnits = false; };			//prompts the cursor to stop group selections
	void setSelectedBuilding(building::building buildingType) { selectedBuilding = buildingType; } //tells teh curso what buildng it has selected

	bool isPlacingBuilding() { return placingBuilding; }; //bool functions to get booleans which show what the cursor is currently doing
	bool isSelectingUnits() { return selectingUnits; };   //

	building::building getSelectedBuilding() { return selectedBuilding; };	//returns selected building

	sf::Vector2f getBoxPosition();			//returns the selection boxes position
	sf::Vector2f getBoxHalfExtents();		//returns teh selection boxes half extents
};

#endif
