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

class Pizza
{
public:
	Pizza(PizzaType pizzaType);
	virtual ~Pizza();

	PizzaType getPizzaType();

	void putInOven();
	void takeOutOfOven();
protected:
	const PizzaType mPizzaType;
	PizzaStatus mPizzaStatus;


};

