#include <pybind11/pybind11.h>

int add(int a, int b)
{
	return a + b;
}

class Animal
{
public:
	virtual ~Animal() {}
	virtual std::string go(int n_times) = 0;
};

class Dog : public Animal
{
	std::string go(int n_times) override
	{
		std::string result;
		for (int i = 0; i < n_times; i++)
		{
			result += u8"ワン! ";
		}
		return result;
	}
};

class PyAnimal : public Animal
{
public:
	using Animal::Animal;

	std::string go(int n_times) override
	{
		PYBIND11_OVERLOAD_PURE(
			std::string,
			Animal,
			go,
			n_times
		);
	}
};

std::string call_go(Animal* animal)
{
	return animal->go(3);
}

namespace py = pybind11;

PYBIND11_PLUGIN(sample02)
{
	py::module m("sample02", "pybind11 module sample.");

	m.def("add", &add, u8"2つの数値を足し合わせます");

	py::class_<Animal, PyAnimal> animal(m, "Animal");
	animal
		.def(py::init<>())
		.def("go", &Animal::go);

	py::class_<Dog> dog(m, "Dog", animal);
	dog
		.def(py::init<>());

	m.def("call_go", &call_go);

	return m.ptr();
}
