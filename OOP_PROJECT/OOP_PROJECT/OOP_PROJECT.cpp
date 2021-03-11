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

	if (number == target) {
		return true;
	}
	return false;
}
int main(array<System::String ^> ^args) {
	int guess;
	do {
		std::cin >> guess;
	} while (!Guess(guess));
	return 0;
}
