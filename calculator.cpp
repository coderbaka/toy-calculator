#include <stack>
#include <cmath>
#include <map>
#include <cctype>
#include <iostream>

#include "calculator.h"

#ifndef NDEBUG
	#define DEBUG(x) x
#else 
	#define DEBUG(x)
#endif

const double PI = std::atan(1)*4;
const double e = std::exp(1);

const std::set<std::string> Calculator::supported_functions_ = {
	"sqrt", "cbrt", "ln",
	"log10", "exp", "add", "sub", "mul", "div",
	"pow", "log", "sin", "cos", "tan", "arcsin", "arccos", "arctan",
        "abs"	
};	
const std::set<std::string> Calculator::binary_real_numeric_functions_ = {
	"add", "sub", "mul", "div", "pow", "log", 
};
const std::set<std::string> Calculator::unary_real_numeric_functions_ = {
	"sqrt", "cbrt", "ln", "log10", "exp", 
	"sin", "cos", "tan",
	"arcsin", "arccos", "arctan", "abs"
};

bool Calculator::is_number(const std::string &str) {
	for (char ch : str)
		if (!std::isdigit(ch) && ch != '.')
			return false;
	return true;
}

static double operand_to_double(const std::string &operand) {
	if (operand == "PI")
		return PI;
	else if (operand == "e")
		return e;
	else {
		try {
			return std::stod(operand);
		}
		catch (const std::invalid_argument &) {
			throw CalculatorInvalidExpressionException(std::string("Not a number: ") + operand );
		}
		catch (const std::out_of_range &) {
			throw CalculatorInvalidExpressionException(std::string("Number too big") + operand );
		}
	}
}

void Calculator::pop_operator(std::stack<std::string> &operators,
		       	std::stack<std::string> &operands) {
	if (operators.empty()) throw CalculatorInvalidExpressionException();
	std::string operator_ = operators.top();

        // Rememer to pop out if the operator is accpeted

	// Deal with 1 argument operators
	if (unary_real_numeric_functions_.find(operator_) != unary_real_numeric_functions_.end()) {
		if (operands.size() < 1) throw CalculatorInvalidExpressionException();
		double operand = operand_to_double(operands.top()); operands.pop();
		
		DEBUG(std::cerr << "Unary Operator Calculate: " << operator_ <<  " " << operand << std::endl;)
		
		double result = NAN;
		if (operator_ == "sqrt")
			result = std::sqrt(operand);
		else if (operator_ == "cbrt")
			result = std::cbrt(operand);
		else if (operator_ == "ln")
			result = std::log(operand);
		else if (operator_ == "log10")
			result = std::log10(operand);
		else if (operator_ == "exp")
			result = std::exp(operand);
		else if (operator_ == "sin")
			result = std::sin(operand);
		else if (operator_ == "cos")
			result = std::cos(operand);
		else if (operator_ == "tan")
			result = std::tan(operand);
		else if (operator_ == "arcsin")
			result = std::asin(operand);
		else if (operator_ == "arccos")
			result = std::acos(operand);
		else if (operator_ == "arctan")
			result = std::atan(operand);
		else if (operator_ == "abs")
			result = std::abs(operand);
		
		operands.push(std::to_string(result));
		operators.pop();
	}	
	// Deal with 2 arguments operators	
	else if (operator_ == "+" || operator_ == "add"
			|| operator_ == "-" || operator_ == "sub" 
			|| operator_ == "*" || operator_ == "mul" 
			|| operator_ == "/" || operator_ == "div"
	  		|| operator_ == "^" || operator_ == "pow" 
			|| operator_ == "log" ) {
		if ((operator_ == "+" || operator_ == "-") && operands.size() == 1) {
			std::string operand_lhs = operands.top(); operands.pop();
			operands.push("0"); operands.push(operand_lhs);
		}

		if (operands.size() < 2) throw CalculatorInvalidExpressionException();
		double operand_rhs = operand_to_double(operands.top()); operands.pop();
		double operand_lhs = operand_to_double(operands.top()); operands.pop();

		DEBUG(std::cerr << "Binary Operator Calculate: " << operand_lhs << " " << operator_ << " " << operand_rhs << std::endl;)

		double result = NAN;
		if (operator_ == "+" || operator_ == "add")
			result = operand_lhs + operand_rhs;
		else if (operator_ == "-" || operator_ == "sub")
			result = operand_lhs - operand_rhs;
		else if (operator_ == "*" || operator_ == "mul")
			result = operand_lhs * operand_rhs;
		else if (operator_ == "/" || operator_ == "div" )
			result = operand_lhs / operand_rhs;
		else if (operator_ == "^" || operator_ == "pow")
			result = std::pow(operand_lhs, operand_rhs);
		else if (operator_ == "log")
			result = std::log(operand_rhs) / std::log(operand_lhs);

		operands.push(std::to_string(result));
		operators.pop();
	}
	else if (operator_ == "(")
		return;

	else {
		throw CalculatorInvalidExpressionException("Error: Unknown operator " + operator_);
	}
	
}

int Calculator::number_of_arguments(const std::string &function_name) const {
	if (binary_real_numeric_functions_.find(function_name)
			!= binary_real_numeric_functions_.end())
		return 2;
	if (unary_real_numeric_functions_.find(function_name)
			!= unary_real_numeric_functions_.end())
		return 1;
	throw std::logic_error("Imporsible to reach: int Calculator::number_of_arguments(const std::string &function_name)");
}

double Calculator::calculate(const std::string end_token) {
	std::stack<std::string> operators;
	std::stack<std::string> operands; 
	
	std::map<std::string, int> operator_priority = {
		{"+", 1},
		{"-", 1},
		{"*", 2},
		{"/", 2},
		{"^", 3},
	};

	while (has_more_token()) {
		std::string next_token = read_next_token();
		if (next_token == end_token)
			break;
		
		if (next_token == "(") {
			double result = calculate(")");
			operands.push(std::to_string(result));
		}

		else if (next_token == "+" || next_token == "-"
				|| next_token == "*"
				|| next_token == "/"
				|| next_token == "^") {
			if (!operators.empty())
				if (operator_priority[operators.top()] 
						>= operator_priority[next_token]) 
					pop_operator(operators, operands);
			operators.push(next_token);
		}
		
		// parse function call
		else if (is_supported_function(next_token)) {
			operators.push(next_token);
			int _number_of_arguments = number_of_arguments(next_token);
			next_token = read_next_token();
			if (next_token != "(")
				throw CalculatorInvalidExpressionException();

			for (int i = 0;i < _number_of_arguments;i++) {
				double result = NAN;
				if (i != _number_of_arguments - 1)
					result = calculate(",");
				else
					result = calculate(")");
				operands.push(std::to_string(result));
			}

			pop_operator(operators, operands);
	
		}
		else
			operands.push(next_token);

	}

	while (!operators.empty())
		pop_operator(operators,operands);

	if (operands.empty()) {
		throw CalculatorInvalidExpressionException();
	}
	return operand_to_double(operands.top());
}

std::string Calculator::read_next_token() {
	std::string result;
	while (has_more_token()) {
		char ch = expression_[i_];
		i_++;
		if (ch == ' ')
			continue;
		else if (ch == '+' || ch == '-' 
			      || ch == '*'
			      || ch == '/'
			      || ch == '^'
			      || ch == '('
			      || ch == ','
			      || ch == ')') {
			result.push_back(ch);
			break;
		}
		else if (std::isdigit(ch)) {
			result.push_back(ch);
			while (has_more_token() && 
					(std::isdigit(expression_[i_]) || expression_[i_] == '.'))
				result.push_back(expression_[i_++]);
			break;
		}
		else {
			result.push_back(ch);
			while (has_more_token() && std::isalpha(expression_[i_]))
				result.push_back(expression_[i_++]);
			break;
		}
	}

	DEBUG(std::cerr << "Read token: " << result << std::endl;)
	return result;
}
