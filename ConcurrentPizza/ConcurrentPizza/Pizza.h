#pragma once

enum PizzaType {
	Capricioasa = 0,
	Carnivora,
	QuattroFormaggi,
	Vegetariana,
	PIZZA_TYPE_COUNT
};

enum PizzaStatus {
	JustDough = 0,
	CookingInOven,
	Done
};

class Person;

class Pizza
{
public:
	Pizza(Person& personRef, PizzaType pizzaType);
	~Pizza();

	PizzaType getPizzaType() const;
	PizzaStatus getPizzaStatus();

	void putInOven();
	void takeOutOfOven();
	Person& getPersonRef() const;
protected:
	const PizzaType mPizzaType;
	PizzaStatus mPizzaStatus;
	Person& mPersonRef;
	std::mutex mMutex;
};

