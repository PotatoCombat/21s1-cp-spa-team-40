#pragma once

#include <string>

using namespace std;

class Tokens {
public:
    // keywords
    inline static string KEYWORD_PROCEDURE = "procedure";
    inline static string KEYWORD_READ = "read";
    inline static string KEYWORD_PRINT = "print";
    inline static string KEYWORD_CALL = "call";
    inline static string KEYWORD_WHILE = "while";
    inline static string KEYWORD_IF = "if";
    inline static string KEYWORD_THEN = "then";
    inline static string KEYWORD_ELSE = "else";

    // semicolon
    inline static string SYMBOL_SEMICOLON = ";"; // terminal

    // braces
    inline static string SYMBOL_OPEN_BRACE = "{";  // terminal
    inline static string SYMBOL_CLOSE_BRACE = "}"; // terminal

    // brackets
    inline static string SYMBOL_OPEN_BRACKET = "(";
    inline static string SYMBOL_CLOSE_BRACKET = ")";

    // assignment
    inline static string SYMBOL_ASSIGN = "=";

    // logical operators
    inline static string SYMBOL_NOT = "!";
    inline static string SYMBOL_AND = "&&";
    inline static string SYMBOL_OR = "||";

    // comparison operators
    inline static string SYMBOL_LESS_THAN = "<";
    inline static string SYMBOL_GREATER_THAN = ">";
    inline static string SYMBOL_LESS_THAN_OR_EQUALS_TO = "<=";
    inline static string SYMBOL_GREATER_THAN_OR_EQUALS_TO = ">=";
    inline static string SYMBOL_EQUALS = "==";
    inline static string SYMBOL_NOT_EQUALS = "!=";

    // arithmetic operators
    inline static string SYMBOL_PLUS = "+";
    inline static string SYMBOL_MINUS = "-";
    inline static string SYMBOL_MULTIPLY = "*";
    inline static string SYMBOL_DIVIDE = "/";
    inline static string SYMBOL_MODULO = "%";

    // semicolon
    static constexpr char CHAR_SEMICOLON = ';';

    // braces
    static constexpr char CHAR_OPEN_BRACE = '{';
    static constexpr char CHAR_CLOSE_BRACE = '}';

    // brackets
    static constexpr char CHAR_OPEN_BRACKET = '(';
    static constexpr char CHAR_CLOSE_BRACKET = ')';

    // logical/comparison/assignment operators
    static constexpr char CHAR_ASSIGN = '=';
    static constexpr char CHAR_AMPERSAND = '&';
    static constexpr char CHAR_BAR = '|';
    static constexpr char CHAR_NOT = '!';
    static constexpr char CHAR_LESS_THAN = '<';
    static constexpr char CHAR_GREATER_THAN = '>';

    // arithmetic operators
    static constexpr char CHAR_PLUS = '+';
    static constexpr char CHAR_MINUS = '-';
    static constexpr char CHAR_MULTIPLY = '*';
    static constexpr char CHAR_DIVIDE = '/';
    static constexpr char CHAR_MODULO = '%';

    static bool isRoundBracket(string input) {
        return input == Tokens::SYMBOL_OPEN_BRACKET || input == Tokens::SYMBOL_CLOSE_BRACKET;
    }

    static bool isArtihmeticOperator(string input) {
        return input == Tokens::SYMBOL_PLUS || input == Tokens::SYMBOL_MINUS ||
               input == Tokens::SYMBOL_MULTIPLY || input == Tokens::SYMBOL_DIVIDE ||
               input == Tokens::SYMBOL_MODULO;
    }

    static bool isComparisonOperator(string input) {
        return input == Tokens::SYMBOL_GREATER_THAN ||
               input == Tokens::SYMBOL_GREATER_THAN_OR_EQUALS_TO ||
               input == Tokens::SYMBOL_LESS_THAN ||
               input == Tokens::SYMBOL_LESS_THAN_OR_EQUALS_TO || input == Tokens::SYMBOL_EQUALS ||
               input == Tokens::SYMBOL_NOT_EQUALS;
    }

    static bool isLogicalOperator(string input) {
        return input == Tokens::SYMBOL_NOT || input == Tokens::SYMBOL_AND ||
               input == Tokens::SYMBOL_OR;
    }

    static bool isOperator(string input) {
        return isLogicalOperator(input) || isComparisonOperator(input) ||
               isArtihmeticOperator(input);
    }

    static bool isValidAssignOperator(string input) { return (isArtihmeticOperator(input)); }

    static bool isValidConditionalOperator(string input) {
        return (isLogicalOperator(input) || isComparisonOperator(input) ||
                isArtihmeticOperator(input));
    }
};
