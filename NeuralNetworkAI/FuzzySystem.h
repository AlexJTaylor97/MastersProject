#ifndef FUZZYSYSTEM_H
#define FUZZYSYSTEM_H
#include <vector>
#include <iostream>

enum membership {veryLow, low, moderate, high, veryHigh};
//0 = very low; 1 = low; 2 = moderate; 3 = high; 4 = very high;

class FuzzySystem
{
public:
	float calculateAggression(float milPercent, float ecoPercent, float defPercent);			//takes in the fuzzy system inputs as parameters and returns the output
	
private:
	std::vector<float> calculateMembership(float percentIn);									//takes in the percent spent as an input and returns a vector of memberships 
	std::vector<float> applyRuleBase();															//applies the rulebase to the memberhsip functions and outputs the output's memberships
	float defuzzify(std::vector<float> outputMemberships);										//takes in the output's memberships and returns the crisp output

	float milSpendingPercent;								//military spending as a %/100
	float ecoSpendingPercent;								//economic spending as a %/100
	float defSpendingPercent;								//defence spending as a %/100

	std::vector<float> militarySpendingMemberships;			//vector of 5 memberships for military spending
	std::vector<float> economicSpendingMemberships;			//vector of 5 memberships for economic spending
	std::vector<float> defenceSpendingMemberships;			//vector of 5 memberships for defence spending
	std::vector<float> aggressionMemberships;				//vector of 5 memberhsips for the aggression output

	bool isVeryLow(std::vector<float> membershipsIn);		//utility functions that checks if a membership vector has a memberhsip in the specific field
	bool isLow(std::vector<float> membershipsIn);			//
	bool isModerate(std::vector<float> membershipsIn);		//
	bool isHigh(std::vector<float> membershipsIn);			//
	bool isVeryHigh(std::vector<float> membershipsIn);		//
};


#endif