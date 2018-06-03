#include "stdafx.h"
#include "Person.h"
#include "PizzaShop.h"

std::atomic<int> Person::sGlobalPersonIdGenerator;

Person::Person() 
	: mId(sGlobalPersonIdGenerator++),
	mIsBeingServed(false)
{

}


Person::~Person()
{
}

void Person::activate(PizzaShop& pizzaShop)
{
	printf("I am Person #%d and I am going to join the queue at PizzaShop#%d\n",
		mId, pizzaShop.getId());
	
	pizzaShop.addPersonToQueue(std::ref(*this));

	std::unique_lock<std::mutex> lk(mPersonMutex);

	mIsBeingServed_CV.wait(lk, [this] {return mIsBeingServed; });

	printf("Yay! I am #%d and I am being served at PizzaShop#%d\n",
		mId, pizzaShop.getId());
}

int Person::getId() const
{
	return mId;
}

void Person::serveCustomer()
{
	mIsBeingServed = true;
	mIsBeingServed_CV.notify_one();
}
