#pragma once
#include "JobQueue.h"
#include <atomic>

class PizzaShop;

class Person :
	public JobQueue
{
public:
	Person();
	virtual ~Person();
	void activate(PizzaShop& pizzaShop);
	int getId() const;
	void serveCustomer();
private:
	const int mId;
	static std::atomic<int> sGlobalPersonIdGenerator;
	std::mutex mPersonMutex;
	bool mIsBeingServed;
	std::condition_variable mIsBeingServed_CV;
};

