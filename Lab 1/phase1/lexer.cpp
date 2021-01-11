/*
 * Ethan Paek
 * TA: Chris Desiniotis
 * COEN 175L: Mon 5:15 PM - 8:00 PM
 * 10 January 2021
 */

# include <cctype>
# include <iostream>
# include <stdio.h>
# include <string>

using namespace std;

string keywords[] = {
        "auto",
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "goto",
        "if",
        "int",
        "long",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "volatile",
        "while"
};

// check if the input string is a keyword or not
bool checkKeyword(string input){
    for (int i = 0; i < 32; i++){
        if (input.compare(keywords[i]) == 0){
            return true;
        }
    }
    return false;
}

string operators[] = {
        "=",
        "|",
        "||",
        "&&",
        "==",
        "!=",
        "<",
        ">",
        "<=",
        ">=",
        "+",
        "-",
        "*",
        "/",
        "%",
        "&",
        "!",
        "++",
        "--",
        ".",
        "->",
        "(",
        ")",
        "[",
        "]",
        "{",
        "}",
        ";",
        ":",
        ","
};

// check if the input string is an operator or not
bool checkOperator(string input){
    for (int i = 0; i < 30; i++){
        if (input.compare(operators[i]) == 0){
            return true;
        }
    }
    return false;
}

int main()
{
    string token;
    int c = cin.get();

    while (!cin.eof()) {
        token.clear();

        // if it's whitespace, we skip over it
        if (isspace(c)) {
            c = cin.get();
        }

        // case for integer
        else if (isdigit(c)) {
            do {
                token += c;
                c = cin.get();
            } while(isdigit(c));
            cout << "int:" << token << endl;
        }

        // check for identifier or keyword
        else if (isalpha(c) || c == '_'){
            do{
                token += c;
                c = cin.get();
            } while(isalnum(c) || c == '_');

            if (checkKeyword(token) == true){
                cout << "keyword:" << token << endl;
            }
            else{
                cout << "identifier:" << token << endl;
            }
        }

        // check for strings and operators
        else if (ispunct(c)){
            // start parsing for strings
            if (c == '"'){
                token += c;
                do {
                    c = cin.get();
                    // if we have a line-continuation character, then we need to keep parsing
                    if(c == '"' && token[(token.length() - 1)] == '\\'){
                        token += c;
                        c = cin.get();
                    }
                    // otherwise, we can stop at the ending double quotation
                    else if (c == '"'){
                        token += c;
                        break;
                    }
                    token += c;
                } while (c != '"');
                cout << "string:" << token << endl;
                c = cin.get();
            }

            // start parsing for comments
            else if (c == '/'){
                token += c;
                c = cin.get();
                if (c == '*'){
                    // keep adding the asterisk until we can see the end of our comments
                    do {
                        token += c;
                        c = cin.get();
                    } while (!(c == '/' && token[token.length() - 1] == '*'));
                    // now that the end is in sight, we can clear our token since we ignore comments
                    token += c;
                    c = cin.get();
                    token.clear();
                }

                // check if we have the division operator: '/'
                if (checkOperator(token) == true){
                    cout << "operator:" << token << endl;
                }
            }

            // it's either an operator or illegal character
            else{
                token += c;
                // create a temporary token to see if the next char forms an operator with a length of 2
                string temp = token;
                c = cin.get();
                temp += c;
                if (checkOperator(temp) == true){
                    // need to move on iterator since we're using temp in this case, not token
                    c = cin.get();
                    cout << "operator:" << temp << endl;
                }
                else if (checkOperator(token) == true){
                    cout << "operator:" << token << endl;
                }
                // illegal char; ignore it
                else{
                    continue;
                }
            }
        }
    }
    return 0;
}
