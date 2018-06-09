#pragma once
#include "stdafx.h"
#include "Pizza.h"
class PizzaShop;

class Oven
{
public:
	Oven(PizzaShop& pizzaShopRef);
	~Oven();

	bool isEmpty();
	bool startPizzaIfEmpty(std::shared_ptr<Pizza> pizzaPtr);
	void activate();
	int getId() const;
protected:
	
	std::mutex mOvenMutex;
	std::shared_ptr<Pizza> mpPizza;
	std::condition_variable mIsThereAnythingToDo_CV;
	PizzaShop& mPizzaShopRef;
private:
	const int mId;
	static std::atomic<int> sGlobalOvenIdGenerator;
};

