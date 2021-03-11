#include "stdafx.h"
#include <string>
#include <iostream>

using namespace System;
using namespace std;

bool Guess(int number) {
	static int target = -1;
	if (target==-1) {
		Random r;
		target = r.Next() % 100 + 1;
	}

	if (number==target) {
		cout << "Correct !!";
		target = -1;
		return true;
	}
	else {
		cout << "Wrong" << endl;
	}
	return false;
}
int main(array<System::String ^> ^args){
	int guess;
	do {
		std::cin >> guess;
	} while (!Guess(guess));
	return 0;
}
