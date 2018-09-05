#pragma once
#include "Component.h"



class ResourceProducerComponent : public Component
{ 
private:
	float gatherTime = 5.0f;	//const to change refinery gather time
	float timer = 0.0f;				//float to store timer 
	bool miningComplete = false;	//boolean to show if timer has run out
public:
	ResourceProducerComponent() {}
	ResourceProducerComponent(float gatherTimeIn) {
		gatherTime = gatherTimeIn;
	}

	void update(float dt) override {
		timer = timer + dt;

		if (timer >= gatherTime) {
			miningComplete = true;
		}
	}
	void message(const std::string m) override {}


	bool isFinishedMining() { return miningComplete; }		//boolean function to see if it has finished mining 

	void completedMining() {
		timer = 0;
		miningComplete = false;
	};									//function to reset its timer
};