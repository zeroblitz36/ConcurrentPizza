#include "stdafx.h"
#include "Person.h"
#include "PizzaShop.h"
#include <chrono>
#include <random>
#include <time.h>

std::atomic<int> Person::sGlobalPersonIdGenerator;

Person::Person() 
	: mId(sGlobalPersonIdGenerator++),
	mIsSittingDown(false)
{

}


Person::~Person()
{
}

void Person::activate(PizzaShop& pizzaShop)
{
	{
	printf("I am Person #%d and I am going to join the queue at PizzaShop#%d\n",
		mId, pizzaShop.getId());

	pizzaShop.addPersonToQueue(std::ref(*this));

	std::unique_lock<std::mutex> lk(mPersonMutex);

	mIsSittingDown_CV.wait(lk, [this] {return mIsSittingDown; });

	printf("Yay! I am #%d and I am being served at PizzaShop#%d\n",
		mId, pizzaShop.getId());
	}
	choosePizza(pizzaShop);
}

int Person::getId() const
{
	return mId;
}

void Person::sitDown()
{
	std::unique_lock<std::mutex> lk(mPersonMutex);
	mIsSittingDown = true;
	mIsSittingDown_CV.notify_one();
}

void Person::setPizza(std::unique_ptr<Pizza> pizzaPointer)
{
	std::unique_lock<std::mutex> lk(mPersonMutex);
	mpPizza = std::move(pizzaPointer);

	printf("I am Person %d, and I am happy that I received my pizza(%d)\n",
		mId,(int)mpPizza->getPizzaType());
}

void Person::choosePizza(PizzaShop & pizzaShop)
{
	std::unique_lock<std::mutex> lk(mPersonMutex);

	using namespace std::chrono_literals;
	printf("I will wait a bit before deciding what Pizza I want\n");
	std::this_thread::sleep_for(1s);

	std::default_random_engine pizzaRandomizer((unsigned)time(0));
	PizzaType pizzaType = (PizzaType)(pizzaRandomizer()%PIZZA_TYPE_COUNT);

	pizzaShop.startPizzaOrderForPerson(*this, pizzaType);
}
