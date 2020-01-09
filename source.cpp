#include <iostream>
#include <string>
#include <vector>
#include <functional>

// LISTA WYCHWYTYWANIA - ROZNE WARIANTY WYKORZYSTANIA

void example1()
{
	int a = 10;
	int b = 20;

	// [] czyli nic nie przechwytujemy
	// auto l1 = [](int x) {return x > (a + b); };  // BLAD

	// [=] czyli wszystkie obiekty z biezacego zakresu przechwytujemy przez kopie
	auto l2 = [=](int x) {return x > (a + b); };
	std::cout << "1 -> " << std::boolalpha << l2(40) << std::endl;

	// brak mozliwosci modyfikowania zmiennych podczas przekazywania przez kopie
	// przy okazji przekazujesz po przecinku konkretne obiekty
	// auto l3 = [a, b](int x) {a = 0; return x > (a + b); };

	// obiekt a bedzie przekazywany przez referencje wiec mozesz modyfikowac
	// oryginal
	auto l3 = [&a, b](int x) {a = 0; return x > (a + b); };
	std::cout << "2 -> " << std::boolalpha << l3(40) << std::endl;

	// slowo kluczowe mutable pozwala modyfikowac ale tylko kopie obiektow
	auto l4 = [=](int x) mutable
	{
		b = 0; 
		std::cout << "KOPIA b = " << b << std::endl;
		return x > (a + b);
	};
	std::cout << l4(40) << std::endl;

	std::cout << "a = " << a << " b = " << b << std::endl;
}

void example2()
{
	int x = 100;
	int y = 200;

	// [&] czyli wszystko przekazujemy przez referencje
	auto l1 = [&](int e)
	{
		x = 0;
		y = 0;
		return e > (x + y);
	};
	std::cout << "1 -> " << std::boolalpha << l1(40) << std::endl;

	std::cout << "x = " << x << " y = " << y << std::endl;

	// [&, x] wszystko przez referencje z wyjatkiem obiektu x ktory jest 
	// przekazywany przez kopie
	// [&, x](int e) {x = 0; y = 0; return e > (x + y); }; // BLAD

	// [=, &x] wszystko przez wartosc tylko x przez referencje
	auto l2 = [=, &x](int e)
	{
		x = 1000;
		return e > (x + y);
	};
	std::cout << "2 -> " << l2(40) << std::endl;

	std::cout << "x = " << x << " y = " << y << std::endl;
}

enum class Modification {INC, DEC};

class Box
{
	int value;
public:

	explicit Box(int new_value)
	{
		if (new_value > 10) {
			// dzieki this mamy dostep do calego zakresu klasy w ktorej sie
			// znajdujemy nawet do skladnikow prywatnych
			auto init_lambda = [this](int value) {this->value = value % 10; };
			init_lambda(new_value);
		} else
		{
			value = new_value;
		}
	}

	void modify(const Modification modification)
	{
		const auto inc_lambda = [this]() {this->value++; };
		const auto dec_lambda = [this]() {this->value--; };
		const auto zero_lambda = [this]() {this->value = 0; };

		switch (modification)
		{
			case Modification::INC:
				inc_lambda();
				break;
			case Modification::DEC:
				dec_lambda();
				break;
			default:
				zero_lambda();
		}
	}

	friend std::ostream& operator<<(std::ostream& out, const Box& box)
	{
		return out << "VALUE = " << box.value << std::endl;
	}

};

int main()
{
	// example1();
	// example2();

	
	Box b1{ 135};
	std::cout << b1 << std::endl;
	b1.modify(Modification::INC);
	std::cout << b1 << std::endl;

	Box b2{ 9 };
	b2.modify(Modification::DEC);
	std::cout << b2 << std::endl;
	

	return 0;
}