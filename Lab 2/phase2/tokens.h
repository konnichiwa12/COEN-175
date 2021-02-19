/*
 * File:	tokens.h
 *
 * Description:	This file contains the token definitions for use by the
 *		lexical analyzer and parser for Simple C.  Single character
 *		tokens use their ASCII values, so we can refer to them
 *		either as character literals or as symbolic names.
 */

# ifndef TOKENS_H
# define TOKENS_H

enum {
    // single character operators
    ASSIGN = '=',
    GREATER = '>',
    LESSER = '<',
    ADD = '+',
    MINUS = '-',
    STAR = '*',
    DIV = '/',
    MOD = '%',
    ADDRESS = '&',
    NOT = '!',
    DECIMAL = '.',
    LPARENT = '(',
    RPARENT = ')',
    LBRACKET = '[',
    RBRACKET = ']',
    LCURLY = '{',
    RCURLY = '}',
    SEMICOLON = ';',
    COLON = ':',
    COMMA = ',',


    // keywords
    AUTO = 256, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DO, DOUBLE,
    ELSE, ENUM, EXTERN, FLOAT, FOR, GOTO, IF, INT, LONG, REGISTER,
    RETURN, SHORT, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH, TYPEDEF,
    UNION, UNSIGNED, VOID, VOLATILE, WHILE,

    // two character operators, ID, num, string, done
    AND, // = 288
    OR,
    DONE, // EOF
    EQUAL,
    UNEQUAL,
    GREATERTHANOREQUAL,
    LESSERTHANOREQUAL,
    INCREMENT,
    DECREMENT,
    POINTER,
    ID,
    NUM,
    STRING
};

# endif /* TOKENS_H */
