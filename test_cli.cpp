#include <iostream>
#include <string>

#include "calculator.h"

int main() {
	while (true) {
		try {
			std::string input;
			std::cout << "> " << std::flush;
			std::getline(std::cin, input);
			if (input == "")
				break;
			Calculator calculator(input);
			std::cout << calculator.calculate() << std::endl;
		} catch (CalculatorInvalidExpressionException &e) {
			std::cerr << e.error_message() << std::endl << std::flush;
		}
	}
	return 0;
}

