#include "stdafx.h"
#include "Person.h"
#include "PizzaShop.h"

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
		printf("[Person %d] I am Person #%d and I am going to join the queue at PizzaShop#%d\n",
			mId, mId, pizzaShop.getId());

		pizzaShop.addPersonToQueue(std::ref(*this));

		std::unique_lock<std::mutex> lk(mPersonMutex);

		mIsSittingDown_CV.wait(lk, [this] {return mIsSittingDown; });

		printf("[Person %d] Yay! I am Person #%d and I have received a seat at PizzaShop #%d\n",
			mId, mId, pizzaShop.getId());
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
	lk.unlock();
	mIsSittingDown_CV.notify_one();
}

void Person::setPizza(std::shared_ptr<Pizza> pizzaPtr)
{
	std::unique_lock<std::mutex> lk(mPersonMutex);
	mPizzaPtr = pizzaPtr;

	assert(nullptr != mPizzaPtr);
	assert(mPizzaType.has_value() && mPizzaType.value() == mPizzaPtr->getPizzaType());

	printf("[Person %d] I am Person %d, and I am happy that I received my pizza(%d)\n",
		mId, mId,(int)(mPizzaPtr->getPizzaType()));

	mPizzaPtr = nullptr;
}

void Person::choosePizza(PizzaShop& pizzaShop)
{
	std::unique_lock<std::mutex> lk(mPersonMutex);

	using namespace std::chrono_literals;
	printf("[Person %d] I will wait a bit before deciding what Pizza I want\n", mId);
	std::this_thread::sleep_for(1s);
	
	assert(!mPizzaType.has_value());

	std::hash<std::thread::id> threadIdHasher;

	std::default_random_engine pizzaRandomizer((unsigned)clock() ^ threadIdHasher(std::this_thread::get_id()));
	mPizzaType = (PizzaType)(pizzaRandomizer()%PIZZA_TYPE_COUNT);

	pizzaShop.startPizzaOrderForPerson(*this, mPizzaType.value());
}
