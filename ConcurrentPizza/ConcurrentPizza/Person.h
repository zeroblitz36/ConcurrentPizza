#pragma once
#include "stdafx.h"
#include "Pizza.h"
class PizzaShop;

class Person
{
public:
	Person() noexcept;
	~Person();
	void activate(PizzaShop& pizzaShop);
	int getId() const;
	void sitDown();
	void setPizza(Pizza* pizzaPtr);
protected:
	void choosePizza(PizzaShop& pizzaShop);
private:
	const int mId;
	static std::atomic<int> sGlobalPersonIdGenerator;
	std::mutex mPersonMutex;
	bool mIsSittingDown;
	std::condition_variable mIsSittingDown_CV;

	std::optional<PizzaType> mPizzaType;
	Pizza* mPizzaPtr;
};
