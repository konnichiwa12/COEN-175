/*
 * File:	lexer.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the lexical analyzer for Simple C.
 *
 *		Extra functionality:
 *		- checking for out of range integer literals
 *		- checking for invalid string literals
 */

# include <set>
# include <cstdio>
# include <cerrno>
# include <cctype>
# include <cstdlib>
# include <iostream>
# include <bits/stdc++.h>
# include "string.h"
# include "lexer.h"
# include "tokens.h"

using namespace std;
int numerrors, lineno = 1;


/* Later, we will associate token values with each keyword */

static set<string> keywords = {
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
    "while",
};


/*
 * Function:	report
 *
 * Description:	Report an error to the standard error prefixed with the
 *		line number.  We'll be using this a lot later with an
 *		optional string argument, but C++'s stupid streams don't do
 *		positional arguments, so we actually resort to snprintf.
 *		You just can't beat C for doing things down and dirty.
 */

void report(const string &str, const string &arg)
{
    char buf[1000];

    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cerr << "line " << lineno << ": " << buf << endl;
    numerrors ++;
}


/*
 * Function:	lexan
 *
 * Description:	Read and tokenize the standard input stream.  The lexeme is
 *		stored in a buffer.
 */

int lexan(string &lexbuf)
{
    static int c = cin.get();
    bool invalid, overflow;
    long val;
    int p;


    /* The invariant here is that the next character has already been read
       and is ready to be classified.  In this way, we eliminate having to
       push back characters onto the stream, merely to read them again. */

    while (!cin.eof()) {
	lexbuf.clear();


	/* Ignore white space */

	while (isspace(c)) {
	    if (c == '\n')
		lineno ++;

	    c = cin.get();
	}


	/* Check for an identifier or a keyword */

	if (isalpha(c) || c == '_') {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isalnum(c) || c == '_');

	    if (keywords.count(lexbuf) > 0) {
            if (lexbuf == "auto") {
                return AUTO;
            }
            if (lexbuf == "break") {
                return BREAK;
            }
            if (lexbuf == "case") {
                return CASE;
            }
            if (lexbuf == "char") {
                return CHAR;
            }
            if (lexbuf == "const") {
                return CONST;
            }
            if (lexbuf == "continue") {
                return CONTINUE;
            }
            if (lexbuf == "default") {
                return DEFAULT;
            }
            if (lexbuf == "do") {
                return DO;
            }
            if (lexbuf == "double") {
                return DOUBLE;
            }
            if (lexbuf == "else") {
                return ELSE;
            }
            if (lexbuf == "enum") {
                return ENUM;
            }
            if (lexbuf == "extern") {
                return EXTERN;
            }
            if (lexbuf == "float") {
                return FLOAT;
            }
            if (lexbuf ==  "for") {
                return FOR;
            }
            if (lexbuf == "goto") {
                return GOTO;
            }
            if (lexbuf == "if") {
                return IF;
            }
            if (lexbuf == "int") {
                return INT;
            }
            if (lexbuf == "long") {
                return LONG;
            }
            if (lexbuf == "register") {
                return REGISTER;
            }
            if (lexbuf == "return") {
                return RETURN;
            }
            if (lexbuf == "short") {
                return SHORT;
            }
            if (lexbuf == "signed") {
                return SIGNED;
            }
            if (lexbuf == "sizeof") {
                return SIZEOF;
            }
            if (lexbuf == "static") {
                return STATIC;
            }
            if (lexbuf == "struct") {
                return STRUCT;
            }
            if (lexbuf == "switch") {
                return SWITCH;
            }
            if (lexbuf == "typedef") {
                return TYPEDEF;
            }
            if (lexbuf == "union") {
                return UNION;
            }
            if (lexbuf == "unsigned") {
                return UNSIGNED;
            }
            if (lexbuf == "void") {
                return VOID;
            }
            if (lexbuf == "volatile") {
                return VOLATILE;
            }
            if(lexbuf == "while") {
                return WHILE;
            }
	    }
	    else {
            return ID;
        }


	/* Check for a number */

	} else if (isdigit(c)) {
	    do {
		lexbuf += c;
		c = cin.get();
	    } while (isdigit(c));

	    errno = 0;
	    val = strtol(lexbuf.c_str(), NULL, 0);

	    if (errno != 0 || val != (int) val)
		report("integer constant too large");

	    return NUM;


	/* There must be an easier way to do this.  It might seem stupid at
	   this point to recognize each token separately, but eventually
	   we'll be returning separate token values to the parser, so we
	   might as well do it now. */

	} else {
	    lexbuf += c;

	    switch(c) {


	    /* Check for '||' */

	    case '|':
		c = cin.get();

		if (c == '|') {
		    lexbuf += c;
		    c = cin.get();
		    return OR;
		}

		break;


	    /* Check for '=' and '==' */

	    case '=':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
		    return EQUAL;
		}

		return ASSIGN;


	    /* Check for '&' and '&&' */

	    case '&':
		c = cin.get();

		if (c == '&') {
		    lexbuf += c;
		    c = cin.get();
		    return AND;
		}

		return ADDRESS;


	    /* Check for '!' and '!=' */

	    case '!':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
		    return UNEQUAL;
		}

		return NOT;


	    /* Check for '<' and '<=' */

	    case '<':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
		    return LESSERTHANOREQUAL;
		}

		return LESSER;


	    /* Check for '>' and '>=' */

	    case '>':
		c = cin.get();

		if (c == '=') {
		    lexbuf += c;
		    c = cin.get();
		    return GREATERTHANOREQUAL;
		}

		return GREATER;


	    /* Check for '-', '--', and '->' */

	    case '-':
		c = cin.get();

		if (c == '-') {
		    lexbuf += c;
		    c = cin.get();
		    return DECREMENT;
		} else if (c == '>') {
		    lexbuf += c;
		    c = cin.get();
		    return POINTER;
		}

		return MINUS;


	    /* Check for '+' and '++' */

	    case '+':
		c = cin.get();

		if (c == '+') {
		    lexbuf += c;
		    c = cin.get();
		    return INCREMENT;
		}

		return ADD;


	    /* Check for simple, single character tokens */

	    case '*':
	        c = cin.get();
	        return STAR;
        case '%':
            c = cin.get();
            return MOD;
        case ':':
            c = cin.get();
            return COLON;
        case ';':
            c = cin.get();
            return SEMICOLON;
	    case '(':
            c = cin.get();
            return LPARENT;
        case ')':
            c = cin.get();
            return RPARENT;
        case '[':
            c = cin.get();
            return LBRACKET;
        case ']':
            c = cin.get();
            return RBRACKET;
	    case '{':
            c = cin.get();
            return LCURLY;
        case '}':
            c = cin.get();
            return RCURLY;
        case '.':
            c = cin.get();
            return DECIMAL;
        case ',':
            c = cin.get();
            return COMMA;


	    /* Check for '/' or a comment */

	    case '/':
		c = cin.get();

		if (c == '*') {
		    do {
			while (c != '*' && !cin.eof()) {
			    if (c == '\n')
				lineno ++;

			    c = cin.get();
			}

			c = cin.get();
		    } while (c != '/' && !cin.eof());

		    c = cin.get();
		    break;

		} else {
		    return DIV;
		}


	    /* Check for a string literal */

	    case '"':
		do {
		    p = c;
		    c = cin.get();
		    lexbuf += c;

		    if (c == '\n')
			lineno ++;

		} while (p == '\\' || (c != '"' && c != '\n' && !cin.eof()));

		if (c == '\n' || cin.eof())
		    report("premature end of string literal");
		else {
		    parseString(lexbuf, invalid, overflow);

		    if (invalid)
			report("unknown escape sequence in string literal");
		    else if (overflow)
			report("escape sequence out of range in string literal");
		}

		c = cin.get();
		return STRING;


	    /* Handle EOF here as well */

	    case EOF:
		    return DONE;


	    /* Everything else is illegal */

	    default:
		c = cin.get();
		break;
	    }
	}
    }

    return DONE;
}


/*
 * Function:	main
 *
 * Description:	Read and tokenize and standard input stream.
 */

/*
int main()
{
    string lexbuf;

    while (lexan(lexbuf))
	continue;

    return 0;
}
*/