#include "stdafx.h"
#include "Pizza.h"

Pizza::Pizza(Person& personRef, PizzaType pizzaType)
	: mPizzaType(pizzaType)
	, mPizzaStatus(JustDough)
	, mPersonRef(personRef)
{
}


Pizza::~Pizza()
{
}

PizzaType Pizza::getPizzaType() const
{
	return mPizzaType;
}

PizzaStatus Pizza::getPizzaStatus()
{
	std::lock_guard<std::mutex> lk(mMutex);
	return mPizzaStatus;
}

void Pizza::putInOven()
{
	std::lock_guard<std::mutex> lk(mMutex);
	assert(mPizzaStatus == JustDough);
	mPizzaStatus = CookingInOven;
}

void Pizza::takeOutOfOven()
{
	std::lock_guard<std::mutex> lk(mMutex);
	assert(mPizzaStatus == CookingInOven);
	mPizzaStatus = Done;
}

Person& Pizza::getPersonRef() const
{
	return mPersonRef;
}
