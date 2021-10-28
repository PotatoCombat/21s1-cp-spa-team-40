#pragma once
#include <string>
using namespace std;

class Tokens {
public:
    static constexpr string_view KEYWORD_PROCEDURE = "procedure";
    static constexpr string_view KEYWORD_ELSE = "else";
    static constexpr string_view KEYWORD_READ = "read";
    static constexpr string_view KEYWORD_PRINT = "print";
    static constexpr string_view KEYWORD_CALL = "call";
    static constexpr string_view KEYWORD_WHILE = "while";
    static constexpr string_view KEYWORD_IF = "if";
    static constexpr string_view KEYWORD_THEN = "then";
    static constexpr string_view SYMBOL_OPEN_BRACKET = "(";
    static constexpr string_view SYMBOL_CLOSE_BRACKET = ")";
    static constexpr string_view SYMBOL_OPEN_BRACE = "{";
    static constexpr string_view SYMBOL_CLOSE_BRACE = "}";
    static constexpr string_view SYMBOL_ASSIGN = "=";
    static constexpr string_view SYMBOL_AMPERSAND = "&";
    static constexpr string_view SYMBOL_BAR = "|";
    static constexpr string_view SYMBOL_NOT = "!";
    static constexpr string_view SYMBOL_LESS_THAN = "<";
    static constexpr string_view SYMBOL_GREATER_THAN = ">";
    static constexpr string_view SYMBOL_PLUS = "+";
    static constexpr string_view SYMBOL_MINUS = "-";
    static constexpr string_view SYMBOL_MULTIPLY = "*";
    static constexpr string_view SYMBOL_DIVIDE = "/";
    static constexpr string_view SYMBOL_MODULO = "%";
    static constexpr string_view SYMBOL_SEMICOLON = ";";
    static constexpr string_view SYMBOL_LESS_THAN_OR_EQUALS_TO = "<=";
    static constexpr string_view SYMBOL_GREATER_THAN_OR_EQUALS_TO = ">=";
    static constexpr string_view SYMBOL_EQUALS = "==";
    static constexpr string_view SYMBOL_NOT_EQUALS = "!=";
    static constexpr string_view SYMBOL_AND = "&&";
    static constexpr string_view SYMBOL_OR = "||";
    static constexpr char CHAR_OPEN_BRACKET = '(';
    static constexpr char CHAR_CLOSE_BRACKET = ')';
    static constexpr char CHAR_OPEN_BRACE = '{';
    static constexpr char CHAR_CLOSE_BRACE = '}';
    static constexpr char CHAR_ASSIGN = '=';
    static constexpr char CHAR_AMPERSAND = '&';
    static constexpr char CHAR_BAR = '|';
    static constexpr char CHAR_NOT = '!';
    static constexpr char CHAR_LESS_THAN = '<';
    static constexpr char CHAR_GREATER_THAN = '>';
    static constexpr char CHAR_PLUS = '+';
    static constexpr char CHAR_MINUS = '-';
    static constexpr char CHAR_MULTIPLY = '*';
    static constexpr char CHAR_DIVIDE = '/';
    static constexpr char CHAR_MODULO = '%';
    static constexpr char CHAR_SEMICOLON = ';';
};