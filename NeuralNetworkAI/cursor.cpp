#include "cursor.h"

Cursor::Cursor() {
	selectionBox.setFillColor(sf::Color(0, 0, 0, 0));
	selectionBox.setOutlineColor(sf::Color::White);
	selectionBox.setOutlineThickness(2);
	selectionBox.setPointCount(4);

	buildingGhost.setFillColor(sf::Color(0, 0, 0, 0));
	buildingGhost.setOutlineColor(sf::Color::Red);
	buildingGhost.setOutlineThickness(2);
}
Cursor::~Cursor() {

}

void Cursor::draw(sf::RenderTarget &target, sf::RenderStates states)const {
	if (placingBuilding) {
		target.draw(buildingGhost);
	}
	if (selectingUnits) {
		target.draw(selectionBox);
	}
}

void Cursor::update(float dt, sf::Vector2f mousePosition) {
	//calculate the positions of the slection circles corner
	if (selectingUnits) {
		xSelectionBounds2 = xSelectionBounds + (mousePosition.x - xSelectionBounds);
		ySelectionBounds2 = ySelectionBounds + (mousePosition.y - ySelectionBounds);

		if (xSelectionBounds2 < 0) {
			xSelectionBounds2 = 0;
		}
		if (xSelectionBounds2 > 3000) {
			xSelectionBounds2 = 3000;
		}
		if (ySelectionBounds2 < 0) {
			ySelectionBounds2 = 0;
		}
		if (ySelectionBounds2 > 3000) {
			ySelectionBounds2 = 3000;
		}

		selectionBox.setPoint(0, sf::Vector2f(xSelectionBounds, ySelectionBounds));
		selectionBox.setPoint(1, sf::Vector2f(xSelectionBounds, ySelectionBounds2));
		selectionBox.setPoint(2, sf::Vector2f(xSelectionBounds2, ySelectionBounds2));
		selectionBox.setPoint(3, sf::Vector2f(xSelectionBounds2, ySelectionBounds));
	}
	//f placing building draw that buildings ghost
	if (placingBuilding) {
		buildingGhost.setPosition(mousePosition.x, mousePosition.y);
	}
}


void Cursor::placeBuilding(sf::RectangleShape buildingIn) {
	//set the transparancy of the building ghost and flip the boolean that tells the cursor to build it
	buildingGhost = buildingIn;
	buildingGhost.setFillColor(sf::Color(30, 145, 255, 124));
	placingBuilding = true;
}

void Cursor::groupSelect(sf::Vector2f mouseLocation) {
	//start to select groups
	xSelectionBounds = mouseLocation.x;
	ySelectionBounds = mouseLocation.y;
	selectingUnits = true;
}

sf::Vector2f Cursor::getBoxPosition() {
	float xDifference = xSelectionBounds2 - xSelectionBounds;
	float yDifference = ySelectionBounds2 - ySelectionBounds;

	return sf::Vector2f(xSelectionBounds + (xDifference / 2), ySelectionBounds + (yDifference / 2));
}
sf::Vector2f Cursor::getBoxHalfExtents() {
	float xDifference = abs(xSelectionBounds2 - xSelectionBounds);
	float yDifference = abs(ySelectionBounds2 - ySelectionBounds);

	return sf::Vector2f (xDifference / 2, yDifference / 2);
}