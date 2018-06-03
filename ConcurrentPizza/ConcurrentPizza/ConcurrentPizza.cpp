// ConcurrentPizza.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PizzaShop.h"
#include "Person.h"
#include <thread>

int main()
{
	printf("Restaurantul de pizza se deschide\n");

	PizzaShop pizzaShop;
	Person person;

	std::thread pizzaShopThread([&]() {
		pizzaShop.activate();
	});

	std::thread personThread([&]() {
		person.activate(pizzaShop);
	});

	pizzaShopThread.join();
	personThread.join();

    return 0;
}

