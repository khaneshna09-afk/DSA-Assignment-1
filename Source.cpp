#include <iostream>
#include <string>
#include <fstream>
using namespace std;
class Stack
{
private:
    string* item;
    int top;
    int size;
public:
    Stack(int s = 100)
    {
        size = s;
        top = -1;
        item = new string[size];
    }
    ~Stack()
    {
        delete[] item;
    }
    bool isEmpty()
    {
        return top == -1;
    }
    bool isFull()
    {
        return top == size - 1;
    }
    void push(string value)
    {
        if (isFull())
        {
            cout << "Stack Overflow!" << endl;
            return;
        }
        top++;
        item[top] = value;
    }
    bool pop(string& value)
    {
        if (isEmpty())
        {
            cout << "Stack Underflow!" << endl;
            return false;
        }
        value = item[top];
        top--;
        return true;
    }
    string peek()
    {
        if (isEmpty())
            return "";

        return item[top];
    }
};
bool isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == '%' || ch == '^')
        return true;

    return false;
}
bool isOpening(char ch)
{
    if (ch == '(' || ch == '[' || ch == '{')
        return true;

    return false;
}
bool isClosing(char ch)
{
    if (ch == ')' || ch == ']' || ch == '}')
        return true;

    return false;
}
bool matching(char open, char close)
{
    if (open == '(' && close == ')')
        return true;
    if (open == '[' && close == ']')
        return true;
    if (open == '{' && close == '}')
        return true;
    return false;
}
int precedence(char ch)
{
    if (ch == '^')
        return 3;
    if (ch == '*' || ch == '/' || ch == '%')
        return 2;
    if (ch == '+' || ch == '-')
        return 1;
    return 0;
}
bool isDigit(char ch)
{
    if (ch >= '0' && ch <= '9')
        return true;

    return false;
}
bool isOperand(string str)
{
    if (str == "")
        return false;
    for (int i = 0; i < str.length(); i++)
    {
        if (!isDigit(str[i]))
            return false;
    }
    return true;
}
int stringToInt(string str)
{
    int number = 0;
    for (int i = 0; i < str.length(); i++)
    {
        number = number * 10 + (str[i] - '0');
    }
    return number;
}
string intToString(int number)
{
    if (number == 0)
        return "0";
    bool negative = false;
    if (number < 0)
    {
        negative = true;
        number = -number;
    }
    string result = "";
    while (number > 0)
    {
        char digit = (number % 10) + '0';
        result = digit + result;
        number = number / 10;
    }
    if (negative)
        result = "-" + result;
    return result;
}
int power(int base, int exponent)
{
    int result = 1;
    for (int i = 0; i < exponent; i++)
    {
        result = result * base;
    }
    return result;
}
bool symbolBalancing(string expression)
{
    Stack s(100);
    for (int i = 0; i < expression.length(); i++)
    {
        char ch = expression[i];
        if (isOpening(ch))
        {
            string temp = "";
            temp = temp + ch;
            s.push(temp);
        }
        else if (isClosing(ch))
        {
            if (s.isEmpty())
                return false;
            string temp;
            s.pop(temp);
            if (!matching(temp[0], ch))
                return false;
        }
    }
    if (s.isEmpty())
        return true;
    return false;
}
string infixToPostfix(string expression)
{
    Stack s(100);
    string postfix = "";
    for (int i = 0; i < expression.length(); i++)
    {
        char ch = expression[i];
        if (ch == ' ')
            continue;
        if ((ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z') ||
            isDigit(ch))
        {
            postfix = postfix + ch;
            postfix = postfix + " ";
        }
        else if (isOpening(ch))
        {
            string temp = "";
            temp = temp + ch;
            s.push(temp);
        }
        else if (isClosing(ch))
        {
            while (!s.isEmpty() && !isOpening(s.peek()[0]))
            {
                string temp;
                s.pop(temp);
                postfix = postfix + temp;
                postfix = postfix + " ";
            }
            if (s.isEmpty())
                return "INVALID EXPRESSION";
            string temp;
            s.pop(temp);
            if (!matching(temp[0], ch))
                return "INVALID EXPRESSION";
        }
        else if (isOperator(ch))
        {
            while (!s.isEmpty() &&
                isOperator(s.peek()[0]) &&
                precedence(s.peek()[0]) >= precedence(ch))
            {
                if (ch == '^' && s.peek()[0] == '^')
                    break;
                string temp;
                s.pop(temp);
                postfix = postfix + temp;
                postfix = postfix + " ";
            }
            string temp = "";
            temp = temp + ch;
            s.push(temp);
        }
        else
        {
            return "INVALID EXPRESSION";
        }
    }
    while (!s.isEmpty())
    {
        string temp;
        s.pop(temp);
        if (isOpening(temp[0]))
            return "INVALID EXPRESSION";
        postfix = postfix + temp;
        postfix = postfix + " ";
    }
    return postfix;
}
string infixToPrefix(string expression)
{
    string postfix = infixToPostfix(expression);
    if (postfix == "INVALID EXPRESSION")
        return "INVALID EXPRESSION";
    string token[100];
    int count = 0;
    string word = "";
    for (int i = 0; i <= postfix.length(); i++)
    {
        if (i == postfix.length() || postfix[i] == ' ')
        {
            if (word != "")
            {
                token[count] = word;
                count++;
                word = "";
            }
        }
        else
        {
            word = word + postfix[i];
        }
    }
    Stack s(100);
    for (int i = 0; i < count; i++)
    {
        if (token[i].length() == 1 &&
            isOperator(token[i][0]))
        {
            string right;
            string left;
            if (!s.pop(right))
                return "INVALID EXPRESSION";
            if (!s.pop(left))
                return "INVALID EXPRESSION";
            string result = token[i] + " " + left + " " + right;
            s.push(result);
        }
        else
        {
            s.push(token[i]);
        }
    }
    string answer;
    if (!s.pop(answer))
        return "INVALID EXPRESSION";
    if (!s.isEmpty())
        return "INVALID EXPRESSION";
    return answer;
}
string postfixToInfix(string expression)
{
    Stack s(100);
    for (int i = 0; i < expression.length(); i++)
    {
        char ch = expression[i];

        if (ch == ' ')
            continue;
        if (isOperator(ch))
        {
            string right;
            string left;
            if (!s.pop(right) || !s.pop(left))
                return "INVALID EXPRESSION";
            string result = "(" + left + " " +
                string(1, ch) + " " + right + ")";
            s.push(result);
        }
        else
        {
            string operand = "";
            operand = operand + ch;
            s.push(operand);
        }
    }
    string answer;
    if (!s.pop(answer))
        return "INVALID EXPRESSION";
    if (!s.isEmpty())
        return "INVALID EXPRESSION";
    return answer;
}
string postfixToPrefix(string expression)
{
    Stack s(100);
    for (int i = 0; i < expression.length(); i++)
    {
        char ch = expression[i];

        if (ch == ' ')
            continue;
        if (isOperator(ch))
        {
            string right;
            string left;
            if (!s.pop(right) || !s.pop(left))
                return "INVALID EXPRESSION";
            string result = string(1, ch) + " " +
                left + " " + right;
            s.push(result);
        }
        else
        {
            string operand = "";
            operand = operand + ch;
            s.push(operand);
        }
    }
    string answer;
    if (!s.pop(answer))
        return "INVALID EXPRESSION";
    if (!s.isEmpty())
        return "INVALID EXPRESSION";
    return answer;
}
string prefixToInfix(string expression)
{
    Stack s(100);
    for (int i = expression.length() - 1; i >= 0; i--)
    {
        char ch = expression[i];
        if (ch == ' ')
            continue;
        if (isOperator(ch))
        {
            string left;
            string right;
            if (!s.pop(left) || !s.pop(right))
                return "INVALID EXPRESSION";
            string result = "(" + left + " " +
                string(1, ch) + " " + right + ")";
            s.push(result);
        }
        else
        {
            string operand = "";
            operand = operand + ch;
            s.push(operand);
        }
    }
    string answer;
    if (!s.pop(answer))
        return "INVALID EXPRESSION";
    if (!s.isEmpty())
        return "INVALID EXPRESSION";
    return answer;
}
string prefixToPostfix(string expression)
{
    Stack s(100);
    for (int i = expression.length() - 1; i >= 0; i--)
    {
        char ch = expression[i];
        if (ch == ' ')
            continue;
        if (isOperator(ch))
        {
            string left;
            string right;
            if (!s.pop(left) || !s.pop(right))
                return "INVALID EXPRESSION";
            string result = left + " " +
                right + " " +
                string(1, ch);
            s.push(result);
        }
        else
        {
            string operand = "";
            operand = operand + ch;
            s.push(operand);
        }
    }
    string answer;
    if (!s.pop(answer))
        return "INVALID EXPRESSION";
    if (!s.isEmpty())
        return "INVALID EXPRESSION";
    return answer;
}
int calculate(int a, int b, char op)
{
    if (op == '+')
        return a + b;
    if (op == '-')
        return a - b;
    if (op == '*')
        return a * b;
    if (op == '/')
    {
        if (b == 0)
        {
            cout << "Error: Division by zero!" << endl;
            return 0;
        }
        return a / b;
    }
    if (op == '%')
    {
        if (b == 0)
        {
            cout << "Error: Modulo by zero!" << endl;
            return 0;
        }
        return a % b;
    }
    if (op == '^')
        return power(a, b);
    return 0;
}
int evaluatePostfix(string expression)
{
    Stack s(100);
    string token = "";
    for (int i = 0; i <= expression.length(); i++)
    {
        if (i == expression.length() || expression[i] == ' ')
        {
            if (token != "")
            {
                if (isOperand(token))
                {
                    s.push(token);
                }
                else if (token.length() == 1 &&
                    isOperator(token[0]))
                {
                    string b;
                    string a;
                    if (!s.pop(b) || !s.pop(a))
                    {
                        cout << "Invalid Postfix Expression!" << endl;
                        return 0;
                    }
                    int num1 = stringToInt(a);
                    int num2 = stringToInt(b);
                    int answer = calculate(num1, num2, token[0]);
                    s.push(intToString(answer));
                }
                else
                {
                    cout << "Invalid Postfix Expression!" << endl;
                    return 0;
                }
                token = "";
            }
        }
        else
        {
            token = token + expression[i];
        }
    }
    string answer;
    if (!s.pop(answer))
    {
        cout << "Invalid Postfix Expression!" << endl;
        return 0;
    }
    if (!s.isEmpty())
    {
        cout << "Invalid Postfix Expression!" << endl;
        return 0;
    }
    return stringToInt(answer);
}
int evaluatePrefix(string expression)
{
    string token[100];
    int count = 0;
    string word = "";
    for (int i = 0; i <= expression.length(); i++)
    {
        if (i == expression.length() || expression[i] == ' ')
        {
            if (word != "")
            {
                token[count] = word;
                count++;
                word = "";
            }
        }
        else
        {
            word = word + expression[i];
        }
    }
    Stack s(100);
    for (int i = count - 1; i >= 0; i--)
    {
        if (isOperand(token[i]))
        {
            s.push(token[i]);
        }
        else if (token[i].length() == 1 &&
            isOperator(token[i][0]))
        {
            string a;
            string b;

            if (!s.pop(a) || !s.pop(b))
            {
                cout << "Invalid Prefix Expression!" << endl;
                return 0;
            }
            int num1 = stringToInt(a);
            int num2 = stringToInt(b);
            int answer = calculate(num1, num2, token[i][0]);
            s.push(intToString(answer));
        }
        else
        {
            cout << "Invalid Prefix Expression!" << endl;
            return 0;
        }
    }
    string answer;
    if (!s.pop(answer))
    {
        cout << "Invalid Prefix Expression!" << endl;
        return 0;
    }
    if (!s.isEmpty())
    {
        cout << "Invalid Prefix Expression!" << endl;
        return 0;
    }
    return stringToInt(answer);
}
void processExpression(string expression, int choice)
{
    if (choice == 1)
    {
        if (symbolBalancing(expression))
            cout << "Result: Symbols are Balanced" << endl;
        else
            cout << "Result: Symbols are NOT Balanced" << endl;
    }
    else if (choice == 2)
    {
        cout << "Postfix: "
            << infixToPostfix(expression) << endl;
    }
    else if (choice == 3)
    {
        cout << "Prefix: "
            << infixToPrefix(expression) << endl;
    }
    else if (choice == 4)
    {
        cout << "Infix: "
            << postfixToInfix(expression) << endl;
    }
    else if (choice == 5)
    {
        cout << "Prefix: "
            << postfixToPrefix(expression) << endl;
    }
    else if (choice == 6)
    {
        cout << "Infix: "
            << prefixToInfix(expression) << endl;
    }
    else if (choice == 7)
    {
        cout << "Postfix: "
            << prefixToPostfix(expression) << endl;
    }
    else if (choice == 8)
    {
        cout << "Result: "
            << evaluatePostfix(expression) << endl;
    }
    else if (choice == 9)
    {
        cout << "Result: "
            << evaluatePrefix(expression) << endl;
    }
}
void fileInput(int choice)
{
    ifstream file("expressions.txt");

    if (!file)
    {
        cout << "Error: expressions.txt not found!" << endl;
        return;
    }
    string expression;
    int number = 1;
    while (getline(file, expression))
    {
        if (expression == "")
            continue;

        cout << "\nExpression " << number << ": "
            << expression << endl;

        processExpression(expression, choice);

        number++;
    }
    file.close();
}
void clearInput()
{
    cin.clear();
    cin.ignore(1000, '\n');
}
int main()
{
    int choice;
    int mode;
    do
    {
        cout << "\n========= EXPRESSION PROCESSOR =========" << endl;
        cout << "1. Check Symbol Balancing" << endl;
        cout << "2. Infix to Postfix" << endl;
        cout << "3. Infix to Prefix" << endl;
        cout << "4. Postfix to Infix" << endl;
        cout << "5. Postfix to Prefix" << endl;
        cout << "6. Prefix to Infix" << endl;
        cout << "7. Prefix to Postfix" << endl;
        cout << "8. Evaluate Postfix" << endl;
        cout << "9. Evaluate Prefix" << endl;
        cout << "10. Exit" << endl;
        cout << "Enter choice: ";
        if (!(cin >> choice))
        {
            cout << "Invalid input! Enter a number." << endl;
            clearInput();
            continue;
        }
        if (choice >= 1 && choice <= 9)
        {
            cout << "\nSelect Input Mode:" << endl;
            cout << "1. Manual Input" << endl;
            cout << "2. File Input" << endl;
            cout << "Enter mode: ";
            if (!(cin >> mode))
            {
                cout << "Invalid input! Enter 1 or 2." << endl;
                clearInput();
                continue;
            }
            if (mode == 1)
            {
                string expression;
                cin.ignore(1000, '\n');
                cout << "Enter expression: ";
                getline(cin, expression);
                processExpression(expression, choice);
            }
            else if (mode == 2)
            {
                fileInput(choice);
            }
            else
            {
                cout << "Invalid Input Mode!" << endl;
            }
        }
        else if (choice == 10)
        {
            cout << "\nProgram Ended." << endl;
        }
        else
        {
            cout << "\nInvalid Choice!" << endl;
        }

    } while (choice != 10);

    return 0;
}