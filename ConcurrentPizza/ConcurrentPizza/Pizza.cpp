#include "stdafx.h"
#include "Pizza.h"
#include <assert.h>

Pizza::Pizza(PizzaType pizzaType)
	: mPizzaType(pizzaType)
	, mPizzaStatus(JustDough)
{
}


Pizza::~Pizza()
{
}

PizzaType Pizza::getPizzaType()
{
	return mPizzaType;
}

void Pizza::putInOven()
{
	assert(mPizzaStatus == JustDough);
	mPizzaStatus = CookingInOven;
}

void Pizza::takeOutOfOven()
{
	assert(mPizzaStatus == CookingInOven);
	mPizzaStatus = Done;
}
