#include <iostream>
#include <stack>
#include <cctype>
using namespace std;

/* ---------- BALANCED PARENTHESES ---------- */
bool isBalanced(string expr) {
    stack<char> s;

    for (char ch : expr) {
        if (ch == '(' || ch == '{' || ch == '[')
            s.push(ch);
        else if (ch == ')' || ch == '}' || ch == ']') {
            if (s.empty()) return false;
            char top = s.top(); s.pop();

            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '['))
                return false;
        }
    }
    return s.empty();
}

/* ---------- OPERATOR UTILITIES ---------- */
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

int applyOperator(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
    }
    return 0;
}

/* ---------- POSTFIX EVALUATION ---------- */
int evaluatePostfix(string expr) {
    stack<int> s;

    for (char ch : expr) {
        if (isdigit(ch))
            s.push(ch - '0');
        else {
            int b = s.top(); s.pop();
            int a = s.top(); s.pop();
            s.push(applyOperator(a, b, ch));
        }
    }
    return s.top();
}

/* ---------- INFIX EVALUATION ---------- */
int evaluateInfix(string expr) {
    stack<int> values;
    stack<char> ops;

    for (int i = 0; i < expr.length(); i++) {
        if (isdigit(expr[i])) {
            values.push(expr[i] - '0');
        }
        else if (expr[i] == '(') {
            ops.push(expr[i]);
        }
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                int b = values.top(); values.pop();
                int a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperator(a, b, op));
            }
            ops.pop();
        }
        else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                int b = values.top(); values.pop();
                int a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperator(a, b, op));
            }
            ops.push(expr[i]);
        }
    }

    while (!ops.empty()) {
        int b = values.top(); values.pop();
        int a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperator(a, b, op));
    }

    return values.top();
}

/* ---------- MAIN MENU ---------- */
int main() {
    int choice;
    string expr;

    do {
        cout << "\n====== Expression Validator & Evaluator ======\n";
        cout << "1. Check Balanced Parentheses\n";
        cout << "2. Evaluate Postfix Expression\n";
        cout << "3. Evaluate Infix Expression\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter expression: ";
                cin >> expr;
                if (isBalanced(expr))
                    cout << "✅ Parentheses are Balanced\n";
                else
                    cout << "❌ Parentheses are NOT Balanced\n";
                break;

            case 2:
                cout << "Enter postfix expression: ";
                cin >> expr;
                cout << "Result = " << evaluatePostfix(expr) << endl;
                break;

            case 3:
                cout << "Enter infix expression: ";
                cin >> expr;
                cout << "Result = " << evaluateInfix(expr) << endl;
                break;

            case 4:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 4);

    return 0;
}
