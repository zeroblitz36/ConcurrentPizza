// ConcurrentPizza.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PizzaShop.h"
#include "Person.h"

int main()
{
	printf("Restaurantul de pizza se deschide\n");

	PizzaShop pizzaShop;

	std::thread pizzaShopThread([&]() {
		pizzaShop.activate();
	});

	std::vector<std::unique_ptr<Person>> persons;
	std::vector<std::thread> personThreads;

	for (int i = 0; i < 10; i++) {
		persons.push_back(std::unique_ptr<Person>(new Person()));
	}
	for (std::unique_ptr<Person> &p : persons) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(50ms);
		personThreads.emplace_back([&p,&pizzaShop]() {
			p->activate(pizzaShop);
		});
	}

	pizzaShopThread.join();
	
	for (std::thread &t : personThreads) {
		t.join();
	}

    return 0;
}

