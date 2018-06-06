#pragma once
#include "JobQueue.h"
#include "Pizza.h"
#include <atomic>
#include <memory>

class PizzaShop;

class Person :
	public JobQueue
{
public:
	Person();
	virtual ~Person();
	void activate(PizzaShop& pizzaShop);
	int getId() const;
	void sitDown();
	void setPizza(std::unique_ptr<Pizza> pizzaPointer);
protected:
	void choosePizza(PizzaShop& pizzaShop);
private:
	const int mId;
	static std::atomic<int> sGlobalPersonIdGenerator;
	std::mutex mPersonMutex;
	bool mIsSittingDown;
	std::condition_variable mIsSittingDown_CV;

	std::unique_ptr<Pizza> mpPizza;
};

