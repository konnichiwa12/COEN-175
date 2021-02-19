/*
 * Ethan Paek
 * TA: Chris Desiniotis
 * COEN 175L: Mon 5:15 PM - 8:00 PM
 * 24 January 2021
 */

# include <iostream>
# include "lexer.h"
# include "tokens.h"

using namespace std;

int lookahead;
string lexbuf;

void expression(); // function definition to avoid compile errors
void statement();

static void error()
{
	if (lookahead == DONE)
		report("syntax error at end of file");
	else
		report("syntax error at '%s'", lexbuf);

	exit(EXIT_FAILURE);
}

static void match(int t)
{
	if (lookahead == t) {
	    lookahead = lexan(lexbuf);
	}
	else {
	    error();
	}
}

void specifier() {
    if (lookahead == INT || lookahead == CHAR || lookahead == VOID) {
        match(lookahead);
    }
}

void pointers() {
    if (lookahead == STAR) {
        match('*');
        pointers();
    }
}

void parameter() {
    specifier();
    pointers();
    match(ID);
}

void parameter_list() {
    parameter();
    if (lookahead == COMMA) {
        match(',');
        parameter_list();
    }
}

void parameters() {
    if (lookahead == VOID) {
        match(VOID);
    }
    else {
        parameter_list();
    }
}

void declarator() {
    pointers();
    match(ID);
    if (lookahead == LBRACKET) {
        match('[');
        match(NUM);
        match(']');
    }
}

void declarator_list() {
    declarator();
    if (lookahead == COMMA) {
        match(',');
        declarator_list();
    }
}

void declaration() {
    specifier();
    declarator_list();
    match(';');
}

void declarations() {
    if (lookahead == INT || lookahead == CHAR || lookahead == VOID) {
        declaration();
        declarations();
    }
}

void assignment() {
    expression();
    while (lookahead == ASSIGN) {
        match(ASSIGN);
        expression();
    }
}

void statements() {
    if (lookahead != RCURLY) {
        statement();
        statements();
    }
}

void statement() {
    if (lookahead == LCURLY) {
        match('{');
        declarations();
        statements();
        match('}');
    }
    else if (lookahead == RETURN) {
        match(RETURN);
        expression();
        match(';');
    }
    else if (lookahead == FOR) {
        match(FOR);
        match('(');
        assignment();
        match(';');
        expression();
        match(';');
        assignment();
        match(')');
        statement();
    }
    else if (lookahead == WHILE || lookahead == IF) {
        int temp = lookahead;
        match(lookahead);
        match('(');
        expression();
        match(')');
        statement();
        if (temp == IF) {
            if (lookahead == ELSE) {
                match(ELSE);
                statement();
            }
        }
    }
    else { // assignment
        assignment();
        match(';');
    }
}

void list_expression() {
    expression();
    if (lookahead == COMMA) {
        match(',');
        list_expression();
    }
}

void other_expressions() {
    if (lookahead == NUM || lookahead == STRING) {
        match(lookahead);
    }
    else if (lookahead == LPARENT) {
        match('(');
        expression();
        match(')');
    }
}

void id_expression() {
    if (lookahead == ID) {
        match(ID);
        if (lookahead == LPARENT) {
            match('(');
            if (lookahead != RPARENT) {
                list_expression();
            }
            if (lookahead == RPARENT) {
                match(')');
            }
        }
    }
    other_expressions();
}

void bracket_expression() {
    id_expression();
    while (lookahead == LBRACKET) {
        match('[');
        expression();
        match(']');
        cout << "index" << endl;
    }
}

void unary_expression() {
    // don't need the bracket expression function here since it's unary
    if (lookahead == ADDRESS || lookahead == STAR || lookahead == NOT || lookahead == MINUS || lookahead == SIZEOF) {
        int temp = lookahead;
        match(lookahead);
        unary_expression();
        if (temp == ADDRESS) {
            cout << "addr" << endl;
        }
        else if (temp == STAR) {
            cout << "deref" << endl;
        }
        else if (temp == NOT) {
            cout << "not" << endl;
        }
        else if (temp == MINUS) {
            cout << "neg" << endl;
        }
        else { // sizeof
            if (lookahead == LPARENT) {
                match('(');
                expression();
                match(')');
            }
            cout << "sizeof" << endl;
        }
    }
    bracket_expression();
}

void mul_or_div_expression() {
    unary_expression();
    while (lookahead == STAR || lookahead == DIV || lookahead == MOD) {
        int temp = lookahead;
        match(lookahead);
        unary_expression();
        if (temp == STAR) {
            cout << "mul" << endl;
        }
        else if (temp == DIV) {
            cout << "div" << endl;
        }
        else { // remainder
            cout << "rem" << endl;
        }
    }
}

void add_or_sub_expression() {
    mul_or_div_expression();
    while (lookahead == ADD || lookahead == MINUS) {
        int temp = lookahead;
        match(lookahead);
        mul_or_div_expression();
        if (temp == ADD) {
            cout << "add" << endl;
        }
        else { // subtraction
            cout << "sub" << endl;
        }
    }
}

void compare_expression() {
    add_or_sub_expression();
    while (lookahead == GREATER || lookahead == LESSER || lookahead == GREATERTHANOREQUAL || lookahead == LESSERTHANOREQUAL) {
        int temp = lookahead;
        match(lookahead);
        add_or_sub_expression();
        if (temp == LESSER) {
            cout << "ltn" << endl;
        }
        else if (temp == GREATER) {
            cout << "gtn" << endl;
        }
        else if (temp == LESSERTHANOREQUAL) {
            cout << "leq" << endl;
        }
        else { // greater than or equal
            cout << "geq" << endl;
        }
    }
}

void equality_expression() {
    compare_expression();
    while (lookahead == EQUAL || lookahead == UNEQUAL) {
        // need a temp since we change the iterator for lookahead
        int temp = lookahead;
        match(lookahead);
        compare_expression();
        if (temp == EQUAL) {
            cout << "eql" << endl;
        }
        else { // unequal
            cout << "neq" << endl;
        }
    }
}

void and_expression() {
    equality_expression();
    while (lookahead == AND) {
        match(AND);
        equality_expression();
        cout << "and" << endl;
    }
}

void expression()
{
	/* Implement recursive-descent parser.
	 * Check slides (Table 1) to get started.
	 */
	and_expression();
    while (lookahead == OR) {
        match(OR);
        and_expression();
        cout << "or" << endl;
    }
}

void global_declarator() {
    pointers();
    match(ID);
    if (lookahead == LPARENT) {
        match('(');
        match(')');
    }
    else if (lookahead == LBRACKET) {
        match('[');
        match(NUM);
        match(']');
    }
}

void remaining_declarations() {
    if (lookahead == COMMA) {
        match(',');
        global_declarator();
        remaining_declarations();
    }
    else if (lookahead == SEMICOLON){
        match(';');
    }
}

void function_or_global() {
    specifier();
    pointers();
    match(ID);
    if (lookahead == LBRACKET) {
        match('[');
        match(NUM);
        match(']');
        remaining_declarations();
    }
    else if (lookahead == LPARENT) {
        match('(');
        if (lookahead == ')') {
            match(')');
            remaining_declarations();
        }
        else {
            parameters();
            match(')');
            match('{');
            declarations();
            statements();
            match('}');
        }
    }
    else {
        remaining_declarations();
    }
}

int main()
{
	lookahead = lexan(lexbuf);

	while (lookahead != DONE) {
	    //declarator_list();
	    function_or_global();
    }

	exit(EXIT_SUCCESS);
}
