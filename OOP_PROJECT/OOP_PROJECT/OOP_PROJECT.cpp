#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

bool Guess(int number) {
	return false;
}

int main(){
	int guess;
	do {
		std::cout << "Choose a number between 1 - 100 :"; std::cin >> guess;
	} while (!Guess(guess));
	return 0;
}
