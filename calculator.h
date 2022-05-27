#ifndef _CPP_CALCULATOR_CALCULATOR_H
#define _CPP_CALCULATOR_CALCULATOR_H

#include <string>
#include <stack>
#include <exception>
#include <set>

class CalculatorInvalidExpressionException: public std::exception {
public:
	CalculatorInvalidExpressionException(
			std::string error_message = "Invalid Expression")
		: error_message_(error_message) {}
	
	std::string error_message() const { return error_message_; }
private:
	std::string error_message_;
};

class Calculator {
public:
	Calculator(const std::string &expression): expression_(expression) {}
	void load(const std::string &expression) { expression_ = expression; }
	double calculate(const std::string end_token="");
	bool is_supported_function(const std::string &function_name) const 
		{ return supported_functions_.find(function_name) != supported_functions_.end(); }
	int number_of_arguments(const std::string &function_name) const;
private:
	static const std::set<std::string> supported_functions_;                     // definition in .cpp file
	static const std::set<std::string> binary_real_numeric_functions_;           // definition in .cpp file
	static const std::set<std::string> unary_real_numeric_functions_;            // definition in .cpp file

	std::string expression_;
	size_t i_ = 0;

	bool has_more_token() const { return i_ < expression_.length(); }
	std::string read_next_token();
	double convert_to_double(const std::string &str);
	void pop_operator(std::stack<std::string> &operators,
		       	std::stack<std::string> &operands);
	bool is_number(const std::string &str);
};



#endif
