#include "stdafx.h"
#include "Pizza.h"
#include "Person.h"

Pizza::Pizza(Person& personRef, PizzaType pizzaType)
	: mPizzaType(pizzaType)
	, mPizzaStatus(JustDough)
	, mPersonRef(personRef)
{
}


Pizza::~Pizza()
{
	printf("Pizza deconstructor was called=> type=%d personId=%d\n",
		mPizzaType,
		mPersonRef.getId());
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
	assert(JustDough == mPizzaStatus);
	mPizzaStatus = CookingInOven;
}

void Pizza::takeOutOfOven()
{
	std::lock_guard<std::mutex> lk(mMutex);
	assert(CookingInOven == mPizzaStatus);
	mPizzaStatus = Done;
}

Person& Pizza::getPersonRef() const
{
	return mPersonRef;
}
