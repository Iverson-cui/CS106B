/*
 * File: main.cpp
 * --------------
 * Blank C++ project configured to use Stanford cslib and Qt
 */

#include "console.h"
#include "simpio.h"
#include "stack.h"
#include "vector.h"
#include "cctype"
#include "strlib.h"
using namespace std;

int calculate(Vector <string> expression);
void input(Vector <string> &input_expression);

int main()
{
    string again;
    do{
    Vector <string> input_expression;
    cout << "Enter your expression character by character. Enter s for stop. Enter l for rewriting the last element. Enter n for inspecting what you have now. " << endl;
    input(input_expression);
    // The function will give you a vector containing strings that are numbers of operators. No other possibilities.
    //string s = "352*+623*/-";
    cout << calculate(input_expression) << endl;
    cout << "Wanna do it again? Press 1 for restart"<< endl;
    getLine(cin,again);
    if (!stringIsInteger(again)) {
        again = "0";
        }
    }while(stringToInteger(again) == 1);
    return 0;
}

int calculate(Vector <string> expression) {
    Stack <int> postfix;  // a empty stack
    for(string s : expression) {
        // This corresponds to s being an int
        if (stringIsInteger(s)) {
            postfix.push(stringToInteger(s));
        }
        // This corresponds to s being a operator and else.
        else{
            // This checks if inserting a operator at this time is legal.
            if(postfix.size() < 2){
                cout << "This is an invalid expression. Result is 1 by default." << endl;
                return 1;
            }
            int right,left;
            switch (stringToChar(s)) {
                case '+':
                    right = postfix.pop();
                    left = postfix.pop();
                    postfix.push(right + left);
                    break;
                case '-':
                    right = postfix.pop();
                    left = postfix.pop();
                    postfix.push(left - right);
                    break;
                case '*':
                    right = postfix.pop();
                    left = postfix.pop();
                    postfix.push(right * left);
                    break;
                case '/':
                    right = postfix.pop();
                    left = postfix.pop();
                    postfix.push(left / right);
                    break;
                default:
                    break;
            }
        }
    }
    // Tell if the stack has one or more elements in the end
    if (postfix.size() == 1){
       return postfix.pop();
    }
    else {
        cout << "This is an invalid expression. Result is 1 by default." << endl;
        return 1;
    }
}
void input(Vector <string> &input_expression) {
    int index = 0;
    while(true) {
        string temp;
        cout << "Enter a character now!!" << endl;
        getLine(cin,temp);
        if(stringIsInteger(temp) || (temp == "+") || (temp == "-") || (temp == "*") || (temp == "/")) {
            cout << "that's good, show me the next one" << endl;
            input_expression.add(temp);
            index++;
        }
        else if (temp == "s") {
            break;
        }
        else if (temp == "l") {
            cout << "You have successfully remove the last element:" << input_expression[index - 1] << endl;
            input_expression.remove(index - 1);
            index = index - 1;
        }
        else if (temp == "n") {
            cout << input_expression << endl;
        }
        else {
            cout << "That's invalid" << endl;
        }
    }
}
