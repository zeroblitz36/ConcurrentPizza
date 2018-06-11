#pragma once
#include "stdafx.h"
#include "ConcurrentQueue.h"
#include "ConcurrentSet.h"
#include "Pizza.h"
#include "Person.h"
#include "Oven.h"

class PizzaShop
{
public:
	PizzaShop() noexcept;
	~PizzaShop();
	
	int getId();
	void activate();
	void addPersonToQueue(Person& person);
	void startPizzaOrderForPerson(Person& person, PizzaType pizzaType);
	void addFinishedPizza(Pizza* pizzaPtr);
	void addAvailableOven(Oven& oven);
protected:

	ConcurrentQueue<std::reference_wrapper<Person>> mPersonQueue;

	struct PizzaOrder{
		std::reference_wrapper<Person> mPerson;
		PizzaType mPizzaType;
	};
	ConcurrentQueue<PizzaOrder> mPizzaOrderQueue;

	ConcurrentQueue <Pizza*> mFinishedPizzasQueue;

	std::vector <std::thread> mOvenThreads;

	ConcurrentQueue<std::reference_wrapper<Oven>> mFreeOvenCollection;
private:
	const int mId;
	static std::atomic<int> sGlobalPizzaShopIdGenerator;

	std::mutex mPizzaShopMutex;
	std::condition_variable mIsThereAnythingToDo_CV;
};

