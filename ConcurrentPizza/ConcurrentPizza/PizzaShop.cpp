#include "stdafx.h"
#include "PizzaShop.h"
#include "Person.h"
#include <chrono>

std::atomic<int> PizzaShop::sGlobalPizzaShopIdGenerator;

PizzaShop::PizzaShop()
	: mId(sGlobalPizzaShopIdGenerator++)
{
}


PizzaShop::~PizzaShop()
{
}

int PizzaShop::getId()
{
	return mId;
}

void PizzaShop::activate()
{
	printf("The pizza shop #%d is now open for business and now waiting for people to come\n",
		mId);

	std::unique_lock<std::mutex> lk(mPizzaShopMutex);

	while (true) {
		printf("Waiting for a customer to join the queue...\n");
		mIsThereAnythingToDo_CV.wait(lk, [this] {return 
			(!mPersonQueue.isEmpty()) ||
			(!mPizzaOrderQueue.isEmpty());
		});

		if (!mPersonQueue.isEmpty())
		{
			printf("A person has joined !\n");
			std::optional<PersonRef> p1 = mPersonQueue.getAndPop();

			if (p1.has_value()) {
				PersonRef p1ref = p1.value().get();
				printf("The persons id is %d, and he will be served!\n", p1ref.get().getId());
				p1ref.get().sitDown();
			}
			else {
				printf("ERROR : The Person Queue was empty !!!\n");
			}
		}
		else if(!mPizzaOrderQueue.isEmpty())
		{
			printf("A person made a pizza order!\n");
			std::optional<PizzaOrder> p1 = mPizzaOrderQueue.getAndPop();

			if (p1.has_value()) {
				PizzaOrder pizzaOrder = p1.value();
				PersonRef personRef = pizzaOrder.mPerson;
				printf("Person %d wants a Pizza(%d), so we send him one\n",
					personRef.get().getId(),
					(int)pizzaOrder.mPizzaType);

				Pizza *brandNewPizza = new Pizza(pizzaOrder.mPizzaType);
				using namespace std::chrono_literals;
				brandNewPizza->putInOven();
				std::this_thread::sleep_for(250ms);
				brandNewPizza->takeOutOfOven();
				std::this_thread::sleep_for(250ms);
				personRef.get().setPizza(std::unique_ptr<Pizza>(brandNewPizza));
			}
		}
	}
}

void PizzaShop::addPersonToQueue(PersonRef person)
{
	std::unique_lock<std::mutex> lk(mPizzaShopMutex);
	int personQueueLength = mPersonQueue.push(person);
	if (1 == personQueueLength) {
		printf("There is a brand new customer ! Wake up the lazy waiter to serve him/her\n");
		mIsThereAnythingToDo_CV.notify_one();
	}
	else if(personQueueLength > 1){
		printf("There are already people at the line. They will have to wait\n");
	}
	else {
		printf("ERROR: mPersonQueue push returned invalid value %d\n", personQueueLength);
	}
}

void PizzaShop::startPizzaOrderForPerson(PersonRef person, PizzaType pizzaType)
{
	std::unique_lock<std::mutex> lk(mPizzaShopMutex);
	int pizzaOrderQueueLength = mPizzaOrderQueue.push({ person,pizzaType });
	if (1 == pizzaOrderQueueLength) {
		printf("There is a brand new pizza order ! Wake up the lazy waiter to take it\n");
		mIsThereAnythingToDo_CV.notify_one();
	}
	else if (pizzaOrderQueueLength > 1) {
		printf("There are already some other people ordering.\n");
	}
	else {
		printf("ERROR: mPizzaOrderQueue push returned invalid value %d\n", pizzaOrderQueueLength);
	}
}

