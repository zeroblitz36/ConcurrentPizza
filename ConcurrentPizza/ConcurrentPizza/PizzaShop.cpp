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
	for (std::thread &th : mOvenThreads) {
		th.join();
	}
}

int PizzaShop::getId()
{
	return mId;
}

void PizzaShop::activate()
{
	std::unique_lock<std::mutex> lk(mPizzaShopMutex);
	printf("[PizzaShop %d] The PizzaShop #%d is now open for business and now waiting for people to come\n",
		mId, mId);

	for (int i = 0; i < 5; i++) {
		mOvenThreads.push_back(std::thread([this]() {
			Oven o(*this);
			o.activate();
		}));
	}

	using namespace std::chrono_literals;
	while (true) {
		mIsThereAnythingToDo_CV.wait(lk, [this] {return 
			(!mPersonQueue.isEmpty()) ||
			(!mPizzaOrderQueue.isEmpty() && !mFreeOvenCollection.isEmpty()) ||
			(!mFinishedPizzasQueue.isEmpty());
		});

		if (!mPersonQueue.isEmpty())
		{
			printf("[PizzaShop %d] A person has joined !\n",
				mId);
			std::optional<std::reference_wrapper<Person>> p1 = mPersonQueue.getAndPop();
			
			assert(p1.has_value());

			std::reference_wrapper<Person> p1ref = p1.value().get();
			printf("[PizzaShop %d] The persons id is %d, and he will be served!\n", mId, p1ref.get().getId());
			p1ref.get().sitDown();
		}
		else if(!mPizzaOrderQueue.isEmpty() && !mFreeOvenCollection.isEmpty())
		{
			printf("[PizzaShop %d] We have a pizza order and an available oven!\n", mId);
			std::optional<PizzaOrder> p1 = mPizzaOrderQueue.getAndPop();
			assert(p1.has_value());
			std::optional<std::reference_wrapper<Oven>> o1 = mFreeOvenCollection.getAndPop();
			assert(o1.has_value());

			PizzaOrder pizzaOrder = p1.value();
			std::reference_wrapper<Person> personRef = pizzaOrder.mPerson;
			std::reference_wrapper<Oven> ovenRef = o1.value();
			printf("[PizzaShop %d] Person %d wants a Pizza(%d), so we start one in Oven #%d\n",
				mId,
				personRef.get().getId(),
				(int)pizzaOrder.mPizzaType,
				ovenRef.get().getId());
			
			bool b = ovenRef.get().startPizzaIfEmpty(std::make_shared<Pizza>(
				personRef, pizzaOrder.mPizzaType
			));
			assert(b);
		}
		else if (!mFinishedPizzasQueue.isEmpty())
		{
			printf("[PizzaShop %d] A pizza was finished!\n", mId);
			std::optional<std::shared_ptr<Pizza>> pizzaPtrOpt = mFinishedPizzasQueue.getAndPop();
			assert(pizzaPtrOpt.has_value());

			std::shared_ptr<Pizza> pizzaPtr = pizzaPtrOpt.value();

			Person& person = pizzaPtr->getPersonRef();
			person.setPizza(pizzaPtr);
		}
	}
}

void PizzaShop::addPersonToQueue(Person& person)
{
	std::unique_lock<std::mutex> lk(mPizzaShopMutex);
	int personQueueLength = mPersonQueue.push(person);
	if (1 == personQueueLength) {
		printf("[PizzaShop %d] There is a brand new customer ! Wake up the lazy waiter to serve him/her\n", mId);
		lk.unlock();
		mIsThereAnythingToDo_CV.notify_one();
	}
	else if(personQueueLength > 1){
		printf("[PizzaShop %d] There are already people at the line. They will have to wait\n", mId);
	}
	else {
		printf("[PizzaShop %d] ERROR: mPersonQueue push returned invalid value %d\n", personQueueLength, mId);
	}
}

void PizzaShop::startPizzaOrderForPerson(Person& person, PizzaType pizzaType)
{
	std::unique_lock<std::mutex> lk(mPizzaShopMutex);

	int pizzaOrderQueueLength = mPizzaOrderQueue.push({ person,pizzaType });
	assert(pizzaOrderQueueLength > 0);
	if (1 == pizzaOrderQueueLength) {
		printf("[PizzaShop %d] There is a brand new pizza order ! Wake up the lazy waiter to take it\n", mId);
		lk.unlock();
		mIsThereAnythingToDo_CV.notify_one();
	}
	else if (pizzaOrderQueueLength > 1) {
		printf("[PizzaShop %d] There are already some other people ordering.\n", mId);
	}
}

void PizzaShop::addFinishedPizza(std::shared_ptr<Pizza> pizzaPtr)
{
	std::unique_lock<std::mutex> lk(mPizzaShopMutex);
	int finishedPizzaQueueLength = mFinishedPizzasQueue.push(pizzaPtr);
	assert(finishedPizzaQueueLength > 0);
	if (1 == finishedPizzaQueueLength) {
		printf("[PizzaShop %d] There is a brand new finished pizza ! Wake up the lazy waiter to take it\n", mId);
		lk.unlock();
		mIsThereAnythingToDo_CV.notify_one();
	}
	else if (finishedPizzaQueueLength > 1) {
		printf("[PizzaShop %d] There are already some other finished pizzas.\n", mId);
	}
}

void PizzaShop::addAvailableOven(Oven & oven)
{
	std::unique_lock<std::mutex> lk(mPizzaShopMutex);
	int availableOvenQueueLength = mFreeOvenCollection.push(oven);
	assert(availableOvenQueueLength > 0);
	if (1 == availableOvenQueueLength) {
		printf("[PizzaShop %d] There is an oven that is now available !\n", mId);
		lk.unlock();
		mIsThereAnythingToDo_CV.notify_one();
	}
	else {
		printf("[PizzaShop %d] There are some available ovens!\n", mId);
	}
}

