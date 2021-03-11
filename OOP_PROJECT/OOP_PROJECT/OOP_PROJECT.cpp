#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>


bool Guess(int number) {
	static int target = -1;
	srand(time(NULL));
	if (target == -1) {
		target = rand() % 100 + 1;
	}

	if (number > target) {
		std::cout << "Smaller" << std::endl;
		return false;
	}
	else if (number < target) {
		std::cout << "Bigger" << std::endl;
		return false;
	}
	return true;

}
int main(array<System::String ^> ^args) {
	int guess;
	do {
		std::cin >> guess;
	} while (!Guess(guess));
	return 0;
}
