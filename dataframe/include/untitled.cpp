#include "exprtk.hpp"
#include <string.h>
#include <iostream>
#include <map>

int main() 
{
	std::string expression_string = "13*pi^2";
	double x;
    exprtk::symbol_table<double> symbol_table;
    symbol_table.add_variable("x",x);
    symbol_table.add_constants();

    exprtk::expression<double> expression;
    expression.register_symbol_table(symbol_table);

    exprtk::parser<double> parser;
    parser.compile(expression_string,expression);

    double y = expression.value();

    std::cout << "Printing answer" + std::to_string(y) << std::endl;

    return 1;
}