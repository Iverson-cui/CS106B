/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include "console.h"
#include "simpio.h"
#include "stack.h"
#include "string.h"
#include "cctype"
#include "strlib.h"
using namespace std;

Vector <string> convert(Vector <string> input_expression);

int main()
{
    Vector <string> expression = {"1","*","(","3","-","4",")","*","5","/","(","4","+","8",")"};
    cout << convert(expression) << endl;
    return 0;
}

Vector <string> convert(Vector <string> input_expression) {
    Stack <char> operators;
    Vector <string> output_expression;
    for (string i : input_expression) {
        // iterate over every element in the input_expression

        // if i is a number
        if ( stringIsInteger(i) ) {
            // This element is a string. So we add it to the output expression vector.
            output_expression.add(i);
        }

        // if i is + or -
        else if (i == "+" || i == "-") {
            // stack is empty: y; peek is other operator: n; peek is bracket: y
            if(operators.isEmpty() || operators.peek() == '(') {
                operators.push(stringToChar(i));
            }
            else {
                output_expression.add(charToString(operators.pop()));
                operators.push(stringToChar(i));
            }
        }

        // if i is * or
        else if (i == "*" || i == "/"){
            if(operators.isEmpty()) {
                operators.push(stringToChar(i));
            }
            else if (operators.peek() == '*' || operators.peek() == '/') {
                output_expression.add(charToString(operators.pop()));
                operators.push(stringToChar(i));
            }
            else {
                operators.push(stringToChar(i));
            }
        }

        // if i is (
        else if (i == "(") {
            // You can try to use recursion.
            operators.push(stringToChar(i));
        }

        // if i is )
        else if (i == ")") {
            do {
                output_expression.add(charToString(operators.pop()));
            } while (operators.peek() != '(');
            operators.pop();
        }
    }
    // When all the input has been processed, there might be some operators left in the stack.
    while( !operators.isEmpty() ) {
        output_expression.add(charToString(operators.pop()));
    }
    return output_expression;
}
