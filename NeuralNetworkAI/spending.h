#ifndef SPENDING_H
#define SPENDING_H

//data storage type has minimal functionality
class Spending
{
private:
	
	char type;				//char to store stype of spending (m,e,d)
	float amount;			//int to store amount spent
	float timer = 20.0f;	//timer to store how long to remember the spending so it remains recent
	bool valid = true;		//bool to switch if teh timer runs out
public:

	Spending(char typeIn, float amountIn) {		//constructor that stores the type and amount
		type = typeIn;
		amount = amountIn;
	}

	char getType() { return type; }				//returns type
	float getAmount() { return amount; }		//returns amount
	bool isValid() { return valid; }			//checks the boolean

	void update(float dt) {				//updates timer to flip boolean if the timer runs out
		timer = timer - dt;
		if (timer < 0) {
			valid = false;
		}
	}
};

#endif