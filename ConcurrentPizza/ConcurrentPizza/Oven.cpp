#include "stdafx.h"
#include "Oven.h"
#include "PizzaShop.h"

std::atomic<int> Oven::sGlobalOvenIdGenerator;

Oven::Oven(PizzaShop& pizzaShopRef)
	: mpPizza(nullptr)
	, mPizzaShopRef(pizzaShopRef)
	, mId(sGlobalOvenIdGenerator++)
{

}


Oven::~Oven()
{
}

bool Oven::isEmpty()
{
	std::unique_lock<std::mutex> lk(mOvenMutex);
	return nullptr != mpPizza;
}

bool Oven::startPizzaIfEmpty(Pizza* pizzaPtr)
{
	std::unique_lock<std::mutex> lk(mOvenMutex);
	if (nullptr != mpPizza) {
		return false;
	}
	mpPizza = pizzaPtr;
	
	lk.unlock();
	mIsThereAnythingToDo_CV.notify_one();
	
	return true;
}

void Oven::activate()
{
	std::unique_lock<std::mutex> lk(mOvenMutex);

	while (true) {
		printf("[Oven %d] Announce PizzaShop #%d that oven #%d is free to use...\n"
			, mId
			, mPizzaShopRef.getId()
			, mId);

		mPizzaShopRef.addAvailableOven(*this);

		mIsThereAnythingToDo_CV.wait(lk, [this] {
			return nullptr != mpPizza;
		});

		assert(PizzaStatus::JustDough == mpPizza->getPizzaStatus());
		//added this assert so that the Code Analyzer stop complaining that there is no
		//NULL check for 'mpPizza'
		assert(nullptr != mpPizza);

		//it's safe to start working on the pizza
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(250ms);
		mpPizza->putInOven();
		std::this_thread::sleep_for(250ms);
		mpPizza->takeOutOfOven();

		printf("[Oven %d] Oven #%d is sending Pizza(%d) back to PizzaShop #%d\n",
			mId,
			mId,
			mpPizza->getPizzaType(),
			mPizzaShopRef.getId());

		mPizzaShopRef.addFinishedPizza(mpPizza);
		mpPizza = nullptr;
	}
}

int Oven::getId() const
{
	return mId;
}


