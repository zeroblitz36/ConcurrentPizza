#pragma once
#include "JobQueue.h"
#include "ConcurrentQueue.h"
#include "Pizza.h"
#include <functional>
#include <atomic>

class Person;

class PizzaShop :
	public JobQueue
{
	typedef std::reference_wrapper<Person> PersonRef;
public:
	PizzaShop();
	virtual ~PizzaShop();
	
	int getId();
	void activate();
	void addPersonToQueue(PersonRef person);
	void startPizzaOrderForPerson(PersonRef person, PizzaType pizzaType);
protected:

	ConcurrentQueue<PersonRef> mPersonQueue;

	struct PizzaOrder{
		PersonRef mPerson;
		PizzaType mPizzaType;
	};
	ConcurrentQueue<PizzaOrder> mPizzaOrderQueue;

private:
	const int mId;
	static std::atomic<int> sGlobalPizzaShopIdGenerator;

	std::mutex mPizzaShopMutex;
	std::condition_variable mIsThereAnythingToDo_CV;
};

