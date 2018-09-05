#include "FuzzySystem.h"
#include <algorithm>

float FuzzySystem::calculateAggression(float milPercent, float ecoPercent, float defPercent) {
	milSpendingPercent = milPercent;
	ecoSpendingPercent = ecoPercent;
	defSpendingPercent = defPercent;

	//calculate memberships
	militarySpendingMemberships = calculateMembership(milSpendingPercent);
	economicSpendingMemberships = calculateMembership(ecoSpendingPercent);
	defenceSpendingMemberships = calculateMembership(defSpendingPercent);

	//apply rule base to memberhsips to populate output functions
	aggressionMemberships = applyRuleBase();

	//defuzzify the output functions 
	return defuzzify(aggressionMemberships);
}


std::vector<float> FuzzySystem::calculateMembership(float percentIn) {
	//calculate memberships
	std::vector<float> memberships;

	for (int i = 0; i < 5; i++) {
		// all membership functions are triangular
		float a, m, b;
		//change the position of the triangualr membership functions depending on which iteration we are on
		//0 = very low; 1 = low; 2 = moderate; 3 = high; 4 = very high;
		switch (i) {
		case(veryLow):
		{
			a = -1;
			m = 0;
			b = 25;
			break;
		}
		case(low):
		{
			a = 0;
			m = 25;
			b = 50;
			break;
		}
		case(moderate):
		{
			a = 25;
			m = 50;
			b = 75;
			break;
		}
		case(high):
		{
			a = 50;
			m = 75;
			b = 100;
			break;
		}
		case(veryHigh):
		{
			a = 75;
			m = 100;
			b = 101;
			break;
		}
		}

		//calculate membership depending on where it falls on this iteration's membership function,
		float membership;
		if (percentIn <= a) {
			membership = 0;
		}

		if (a < percentIn && percentIn <= m) {
			membership = (percentIn - a) / (m - a);
		}

		if (m < percentIn && percentIn < b) {
			membership = (b - percentIn) / (b - m);
		}

		if (percentIn >= b) {
			membership = 0;
		}
		memberships.push_back(membership);
	}
	return memberships;
}

std::vector<float> FuzzySystem::applyRuleBase() {
	std::vector<float> memberships;
	memberships.push_back(0.0f);
	memberships.push_back(0.0f);
	memberships.push_back(0.0f);
	memberships.push_back(0.0f);
	memberships.push_back(0.0f);

	//Military spending THEN rules
	if (isVeryLow(militarySpendingMemberships)) {
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(militarySpendingMemberships.at(veryLow), memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = militarySpendingMemberships.at(veryLow);
		}
	}
	if (isLow(militarySpendingMemberships)) {
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(militarySpendingMemberships.at(low), memberships.at(low));
		}
		else {
			memberships.at(low) = militarySpendingMemberships.at(low);
		}
		
	}
	if (isModerate(militarySpendingMemberships)) {
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(militarySpendingMemberships.at(moderate), memberships.at(moderate));
		} else {
			memberships.at(moderate) = militarySpendingMemberships.at(moderate);
		}
	}
	if (isHigh(militarySpendingMemberships)) {
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(militarySpendingMemberships.at(high), memberships.at(high));
		} else {
			memberships.at(high) = militarySpendingMemberships.at(high);
		}
	}
	if (isVeryHigh(militarySpendingMemberships)) {
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(militarySpendingMemberships.at(veryHigh), memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = militarySpendingMemberships.at(veryHigh);
		}
	}
	//Defence spending THEN rules
	if (isVeryLow(defenceSpendingMemberships)) {
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(defenceSpendingMemberships.at(veryLow), memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = defenceSpendingMemberships.at(veryLow);
		}
	}
	if (isLow(defenceSpendingMemberships)) {
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(defenceSpendingMemberships.at(low), memberships.at(high));
		} else {
			memberships.at(high) = defenceSpendingMemberships.at(low);
		}
	}
	if (isModerate(defenceSpendingMemberships)) {
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(defenceSpendingMemberships.at(moderate), memberships.at(moderate));
		} else {
			memberships.at(moderate) = defenceSpendingMemberships.at(moderate);
		}
	}

	if (isHigh(defenceSpendingMemberships)) {
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(defenceSpendingMemberships.at(high), memberships.at(low));
		} else {
			memberships.at(low) = defenceSpendingMemberships.at(high);
		}
	}

	if (isVeryHigh(defenceSpendingMemberships)) {
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(defenceSpendingMemberships.at(veryHigh), memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = defenceSpendingMemberships.at(veryHigh);
		}
	}

	float value; //temp variable used to store and operators surviving value for the implication operator
	//military spending and rules
	if (isVeryLow(militarySpendingMemberships) && isVeryHigh(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), economicSpendingMemberships.at(veryHigh));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}

	if (isLow(militarySpendingMemberships) && isVeryHigh(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), economicSpendingMemberships.at(veryHigh));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}

	if (isVeryLow(militarySpendingMemberships) && isHigh(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), economicSpendingMemberships.at(high));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isHigh(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), economicSpendingMemberships.at(high));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isHigh(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), economicSpendingMemberships.at(high));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryLow(militarySpendingMemberships) && isModerate(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), economicSpendingMemberships.at(moderate));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isModerate(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), economicSpendingMemberships.at(moderate));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isModerate(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), economicSpendingMemberships.at(moderate));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isHigh(militarySpendingMemberships) && isModerate(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(high), economicSpendingMemberships.at(moderate));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryLow(militarySpendingMemberships) && isLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), economicSpendingMemberships.at(low));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), economicSpendingMemberships.at(low));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), economicSpendingMemberships.at(low));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isHigh(militarySpendingMemberships) && isLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(high), economicSpendingMemberships.at(low));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryHigh(militarySpendingMemberships) && isLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryHigh), economicSpendingMemberships.at(low));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryLow(militarySpendingMemberships) && isVeryLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), economicSpendingMemberships.at(veryLow));
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(veryLow) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isVeryLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), economicSpendingMemberships.at(veryLow));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isVeryLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), economicSpendingMemberships.at(veryLow));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isHigh(militarySpendingMemberships) && isVeryLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(high), economicSpendingMemberships.at(veryLow));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isVeryHigh(militarySpendingMemberships) && isVeryLow(economicSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryHigh), economicSpendingMemberships.at(veryLow));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	//Economic and defence memberships
	if (isVeryLow(economicSpendingMemberships) && isVeryHigh(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(veryLow), defenceSpendingMemberships.at(veryHigh));
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(value, memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = value;
		}
	}
	if (isLow(economicSpendingMemberships) && isVeryHigh(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(low), defenceSpendingMemberships.at(veryHigh));
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(value, memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = value;
		}
	}
	if (isVeryLow(economicSpendingMemberships) && isHigh(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(veryLow), defenceSpendingMemberships.at(high));
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(value, memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = value;
		}
	}
	if (isLow(economicSpendingMemberships) && isHigh(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(low), defenceSpendingMemberships.at(high));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isModerate(economicSpendingMemberships) && isHigh(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(moderate), defenceSpendingMemberships.at(high));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isVeryLow(economicSpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(veryLow), defenceSpendingMemberships.at(moderate));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isLow(economicSpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(low), defenceSpendingMemberships.at(moderate));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isModerate(economicSpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(moderate), defenceSpendingMemberships.at(moderate));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isHigh(economicSpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(high), defenceSpendingMemberships.at(moderate));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isVeryLow(economicSpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(veryLow), defenceSpendingMemberships.at(low));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isLow(economicSpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(low), defenceSpendingMemberships.at(low));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isModerate(economicSpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(moderate), defenceSpendingMemberships.at(low));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isHigh(economicSpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(high), defenceSpendingMemberships.at(low));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isVeryHigh(economicSpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(veryHigh), defenceSpendingMemberships.at(low));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isVeryLow(economicSpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(veryLow), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isLow(economicSpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(low), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isModerate(economicSpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(moderate), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isHigh(economicSpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(high), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryHigh(economicSpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(economicSpendingMemberships.at(veryHigh), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	//Military and defence memberships
	if (isVeryLow(militarySpendingMemberships) && isVeryHigh(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), defenceSpendingMemberships.at(veryHigh));
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(value, memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isVeryHigh(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), defenceSpendingMemberships.at(veryHigh));
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(value, memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = value;
		}
	}
	if (isVeryLow(militarySpendingMemberships) && isHigh(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), defenceSpendingMemberships.at(high));
		if (memberships.at(veryLow) > 0) {
			memberships.at(veryLow) = std::min(value, memberships.at(veryLow));
		} else {
			memberships.at(veryLow) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isHigh(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), defenceSpendingMemberships.at(high));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isHigh(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), defenceSpendingMemberships.at(high));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isVeryLow(militarySpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), defenceSpendingMemberships.at(moderate));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), defenceSpendingMemberships.at(moderate));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), defenceSpendingMemberships.at(moderate));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isHigh(militarySpendingMemberships) && isModerate(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(high), defenceSpendingMemberships.at(moderate));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isVeryLow(militarySpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), defenceSpendingMemberships.at(low));
		if (memberships.at(moderate) > 0) {
			memberships.at(moderate) = std::min(value, memberships.at(moderate));
		} else {
			memberships.at(moderate) = value;
		}
	}
	if (isLow(militarySpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), defenceSpendingMemberships.at(low));
		if (memberships.at(low) > 0) {
			memberships.at(low) = std::min(value, memberships.at(low));
		} else {
			memberships.at(low) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), defenceSpendingMemberships.at(low));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isHigh(militarySpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(high), defenceSpendingMemberships.at(low));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryHigh(militarySpendingMemberships) && isLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryHigh), defenceSpendingMemberships.at(low));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryLow(militarySpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryLow), defenceSpendingMemberships.at(veryLow));
		memberships.at(low) = std::min(value, memberships.at(low));
	}
	if (isLow(militarySpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(low), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(high) > 0) {
			memberships.at(high) = std::min(value, memberships.at(high));
		} else {
			memberships.at(high) = value;
		}
	}
	if (isModerate(militarySpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(moderate), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isHigh(militarySpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(high), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	if (isVeryHigh(militarySpendingMemberships) && isVeryLow(defenceSpendingMemberships)) {
		value = std::min(militarySpendingMemberships.at(veryHigh), defenceSpendingMemberships.at(veryLow));
		if (memberships.at(veryHigh) > 0) {
			memberships.at(veryHigh) = std::min(value, memberships.at(veryHigh));
		} else {
			memberships.at(veryHigh) = value;
		}
	}
	return memberships;

}

float FuzzySystem::defuzzify(std::vector<float> outputMemberships) {
	
	//weighed average method
	
	//centroid using weighted average method uses 5 trapezoidal output functions
	float centerOfVeryLow = 0;
	float centerOfLow = 25;
	float centerOfModerate = 50;
	float centerOfHigh = 75;
	float centerOfVeryHigh = 100;
	//for readability i seperated out the weighted avererage equation. 
	float topHalfOfEquation = (outputMemberships.at(veryLow) * centerOfVeryLow) + (outputMemberships.at(low) * centerOfLow) + (outputMemberships.at(moderate) * centerOfModerate) + (outputMemberships.at(high) * centerOfHigh) + (outputMemberships.at(veryHigh) * centerOfVeryHigh);
	float bottomHalfOfequation = outputMemberships.at(veryLow) + outputMemberships.at(low) + outputMemberships.at(moderate) + outputMemberships.at(high) + outputMemberships.at(veryHigh);
	return (topHalfOfEquation / bottomHalfOfequation);

}

bool FuzzySystem::isVeryLow(std::vector<float> membershipsIn) {
	if (membershipsIn.at(veryLow) > 0) {
		return true;
	} else {
		return false;
	}
}

bool FuzzySystem::isLow(std::vector<float> membershipsIn) {
	if (membershipsIn.at(low) > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool FuzzySystem::isModerate(std::vector<float> membershipsIn) {
	if (membershipsIn.at(moderate) > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool FuzzySystem::isHigh(std::vector<float> membershipsIn) {
	if (membershipsIn.at(high) > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool FuzzySystem::isVeryHigh(std::vector<float> membershipsIn) {
	if (membershipsIn.at(veryHigh) > 0) {
		return true;
	}
	else {
		return false;
	}
}