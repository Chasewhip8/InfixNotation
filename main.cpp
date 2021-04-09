#include <iostream>
#include <string>
#include <utility>
#include "DynamicStack.h"
#include "cmath"
#include "algorithm"

using namespace std;

class ExpressionEvaluationException: exception {
private:
    const char *msg;

    const char *what() const override {
        return msg;
    }

public:
    explicit ExpressionEvaluationException(const char *m = "Unknown Exception"){
        this->msg = m;
    }
};

void replaceBrackets(string &str){
    replace(str.begin(), str.end(), '[', '(');
    replace(str.begin(), str.end(), ']', ')');
    replace(str.begin(), str.end(), '{', '(');
    replace(str.begin(), str.end(), '}', ')');
}

/*
 * String needs to be pre-processed using replaceBrackets;
 */
bool checkBalanced(string &str){
    DynamicStack<char> bracketStack;

    for (char c : str) {
        switch (c) {
            case '(':
                bracketStack.push(')');
                break;
            case ')':
                if (bracketStack.pop() != c){
                    return false;
                }

                break;
            default:
                break;
        }
    }

    return true;
}

int precedence(char c){
    switch (c) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '(':
        case ')':
            return -1;
        default:
            return 0;
    }
}

double solve(char c, double x, double y){
    cout << endl << "Solving: " << x << " " << c << " " << y;

    switch (c) {
        case '^':
            return pow(x, y);
        case '*':
            return x * y;
        case '/':
            return x / y;
        case '+':
            return x + y;
        case '-':
            return x - y;
        default:
            throw ExpressionEvaluationException("Unhandled Operation Found");
    }
}

/*
 * Ops
 *  ( ) ^ * / + -
 */
double evaluateInfix(string &infix){
    replaceBrackets(infix);

    cout << infix;

    if (!checkBalanced(infix)){
        throw ExpressionEvaluationException("Unbalanced Equation");
    }

    DynamicStack<char> opStack;
    DynamicStack<double> dataStack;

    bool lastPushedOp = true; // Set to true in order to satisfy checks that require there to be a real data set on the stack
    bool lastPushedData = false;
    bool lastFoundDecimal = false; // If true any newly found data will be added as the next decimal place, reset when next op is found
    int currentDecimal = 0;

    for (char c : infix) { // Move ops and data into stack
        switch (c) {
            case ' ':
                break;
            case '^':
            case '*':
            case '/':
            case '+':
            case '-':
                if (lastPushedOp && !(opStack.peek() == '(' || opStack.peek() == ')')){
                    throw ExpressionEvaluationException("Two or more operations repeating without data");
                }
            case '(':
            case ')':
                opStack.push(c);

                lastPushedOp = true;
                lastFoundDecimal = false;
                lastPushedData = false;
                break;
            case '.':
                if (!lastPushedData){
                    throw ExpressionEvaluationException("Leading decimal without starting number.");
                }

                lastFoundDecimal = true;
                currentDecimal = 1;
                lastPushedOp = false;
                break;
            default:
                int value = c - '0';

                if (lastPushedOp) {
                    dataStack.push(value);

                    lastPushedData = true;
                    lastPushedOp = false;
                    currentDecimal = 1;
                    break;
                }

                if (lastFoundDecimal){
                    double d = dataStack.pop();

                    d += (value * pow(10, -currentDecimal));
                    currentDecimal++;

                    dataStack.push(d);

                    lastPushedData = true;
                    lastPushedOp = false;
                    break;
                }

                if (lastPushedData){
                    double d = dataStack.pop();

                    d = (d * 10) + value; // We read from left to right so this is different than adding a decimal.

                    currentDecimal++;

                    dataStack.push(d);

                    lastPushedData = true;
                    lastPushedOp = false;
                    break;
                }

                throw ExpressionEvaluationException(&"Data not pushed to stack: " [ c]);
        }
    }

    bool inSet = false; // inside ( )
    char afterOP;
    double afterValue;

    while (!opStack.isEmpty()) {
        char op = opStack.pop();
        char nextOp;
        if (!opStack.isEmpty()) {
            nextOp = opStack.peek();
        } else {
            nextOp = ' '; // set to something random to satisfy precedence checks, wont get pushed to opStack
        }

        if (op == '(') {
            if (inSet){
                opStack.push(afterOP);
                dataStack.push(afterValue);
            }

            inSet = false;
            continue;
        }

        if (nextOp == ')') {
            inSet = true;
            afterOP = op;
            afterValue = dataStack.pop();
            continue;
        }

        if (op == ')') {
            continue;
        }

        double second;
        double first;
        double x;

        if (precedence(nextOp) > precedence(op)){
            opStack.pop();
            opStack.push(op);
            double tempPop = dataStack.pop();

            second = dataStack.pop();
            first = dataStack.pop();

            x = solve(nextOp, first, second);

            dataStack.push(tempPop); // Push old first to preserve order
            dataStack.push(x); // push value and repeat
        } else {
            second = dataStack.pop();
            first = dataStack.pop();

            x = solve(op, first, second);
            dataStack.push(x); // push value and repeat
        }
    }

    cout << endl;

    cout << "OpStack print out: " << endl;

    while (opStack.size() > 0){
        cout << opStack.pop() << ", ";
    }

    cout << endl;

    cout << "DataStack print out: " << endl;
    while (dataStack.size() > 0){
        cout << dataStack.pop() << ", ";
    }
}


int main() {
    string expression;
    cout << "Enter and expression: ";
    getline(cin, expression);

    evaluateInfix(expression);

    return 0;
}
