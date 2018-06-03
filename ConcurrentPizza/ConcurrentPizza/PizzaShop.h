#pragma once
#include "JobQueue.h"
#include "ConcurrentQueue.h"
#include <functional>
#include <atomic>

class Person;

class PizzaShop :
	public JobQueue
{
public:
	PizzaShop();
	virtual ~PizzaShop();
	
	int getId();
	void activate();
	void addPersonToQueue(std::reference_wrapper<Person> person);
protected:
	ConcurrentQueue<std::reference_wrapper<Person>> mPersonQueue;

private:
	const int mId;
	static std::atomic<int> sGlobalPizzaShopIdGenerator;

	std::mutex mPizzaShopMutex;
	std::condition_variable mAreTherePersonsInTheQueue_CV;
};

