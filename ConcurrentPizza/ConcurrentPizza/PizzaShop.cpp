#include "stdafx.h"
#include "PizzaShop.h"
#include "Person.h"

std::atomic<int> PizzaShop::sGlobalPizzaShopIdGenerator;

PizzaShop::PizzaShop()
	: mId(sGlobalPizzaShopIdGenerator++)
{
}


PizzaShop::~PizzaShop()
{
}

int PizzaShop::getId()
{
	return mId;
}

void PizzaShop::activate()
{
	printf("The pizza shop #%d is now open for business and now waiting for people to come\n",
		mId);

	std::unique_lock<std::mutex> lk(mPizzaShopMutex);

	mAreTherePersonsInTheQueue_CV.wait(lk, [this] {return !mPersonQueue.isEmpty(); });

	printf("A person has joined !\n");
	std::optional<std::reference_wrapper< Person>> p1 = mPersonQueue.getAndPop();

	if (p1.has_value()) {
		printf("The persons id is %d, and he will be served!\n", p1.value().get().getId());
	}
	else {
		printf("ERROR : The Person Queue was empty !!!\n");
	}
}

void PizzaShop::addPersonToQueue(std::reference_wrapper<Person> person)
{
	int personQueueLength = mPersonQueue.push(person);
	if (1 == personQueueLength) {
		printf("There is a brand new customer ! Wake up the lazy waiter to serve him/her\n");
	}
	else if(personQueueLength > 1){
		printf("There are already people at the line. They will have to wait\n");
	}
	else {
		printf("ERROR: mPersonQueue push returned invalid value %d\n", personQueueLength);
	}
}

