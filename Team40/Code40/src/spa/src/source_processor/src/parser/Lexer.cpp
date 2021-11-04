#include "source_processor/parser/Lexer.h"
#include "source_processor/parser/Tokens.h"
#include <algorithm>
#include <iostream>
#include <tuple>

// tokenize file input

/**
 * Tokenizes an input file using lexical analysis into its respective program lines and assigns
 * each line a respective index
 * @return a list of tokenized program lines
 */
vector<Line> Lexer::tokenizeFile(fstream &file) {
    vector<Line> programLines;
    vector<vector<string>> programTokens = mergeLine(file);
    int stmtNum = 0;
    vector<string> currString;
    for (int i = 0; i < programTokens.size(); i++) {
        vector<string> subString = programTokens[i];
        currString.insert(currString.end(), subString.begin(), subString.end());

        if (!isCurlyBracket(currString.back()) && !isSemiColon(currString.back())) {
            continue;
        }

        if (isStmt(currString)) {
            stmtNum++;
        }

        Line curr = Line(stmtNum, currString);
        programLines.push_back(curr);
        currString = {};
    }

    if (!brackets.empty()) {
        throw invalid_argument("invalid program, brackets do not match");
    }
    return programLines;
}

/**
 * Merges tokenized program lines that have been separated in the source file
 * @return a list of tokenized program lines without an index
 */
vector<vector<string>> Lexer::mergeLine(fstream &file) {
    string input;
    vector<vector<string>> programTokens;

    while (getline(file, input)) {
        vector<string> line = tokenizeLine(input);
        tuple<vector<string>, vector<string>> splitString = splitLine(line);
        vector<string> currString = get<0>(splitString);
        vector<string> nextString = get<1>(splitString);

        if (!currString.empty()) {
            programTokens.push_back(currString);
        }

        while (nextString.size() > 0) {
            splitString = splitLine(nextString);
            currString = get<0>(splitString);
            nextString = get<1>(splitString);

            if (!currString.empty()) {
                programTokens.push_back(currString);
            }
        }
    }
    return programTokens;
}

/**
 * Splits a tokenized program line according to terminal tokens (i.e. semicolons and braces)
 * @return a tuple with the first and second element representing the input program line after the
 * first split
 */
tuple<vector<string>, vector<string>> Lexer::splitLine(vector<string> line) {
    tuple<vector<string>, vector<string>> splitString;
    vector<string> currString;
    vector<string> nextString;

    for (int i = 0; i < line.size(); i++) {
        string curr = line[i];
        if ((isCurlyBracket(curr)) || isSemiColon(curr)) {
            currString.push_back(curr);
            nextString = vector<string>(line.begin() + i + 1, line.end());
            break;
        }
        currString.push_back(curr);
    }

    get<0>(splitString) = currString;
    get<1>(splitString) = nextString;
    return splitString;
}

/**
 * Tokenizes an input file line according to the parsing expression grammar
 * @return a tokenized version of the input file line
 */
vector<string> Lexer::tokenizeLine(string input) {
    vector<string> inputLine;
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);
        currString = cleanString(currString);

        if (isOperator(curr) || isBracket(curr) || isSemiColon(curr)) {
            // push back previous string before special character
            addString(currString, inputLine);
            currString.push_back(curr);
            tokenizeAndAddSymbol(input, i, curr, currString, inputLine);
            checkValidBracket(curr);

        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                tokenizeAndAddKeyword(input, i, currString, inputLine);
            }
        }
    }

    currString = cleanString(currString);
    if (!currString.empty()) {
        inputLine.push_back(currString);
    }
    return inputLine;
}

// helper functions

/**
 * Tokenizes an operator symbol and add its to the list of tokens from the current input file line
 */
void Lexer::tokenizeAndAddSymbol(string input, int &i, char curr, string &currString,
                                 vector<string> &inputLine) {
    // check if operator has an additional character
    if (i < input.size() - 1) {
        char next = input.at(i + 1);
        if (isOperator(next) && isOperator(curr)) {
            currString.push_back(next);
            i++;
        }
    }
    addString(currString, inputLine);
}

/**
 * Tokenizes a keyword and adds it to the list of tokens from the current input file line
 */
void Lexer::tokenizeAndAddKeyword(string input, int &i, string &currString,
                                  vector<string> &inputLine) {
    // check if keyword has an additional character
    if (i < input.size() - 1) {
        if (!isalnum(input.at(i + 1))) {
            addString(currString, inputLine);
        }
    } else {
        addString(currString, inputLine);
    }
}

/**
 * Adds the current token to the list of tokens from the current input file line and resets the
 * input string to be empty
 */
void Lexer::addString(string &input, vector<string> &inputVector) {
    input = cleanString(input);
    if (!input.empty()) {
        inputVector.push_back(input);
    }
    input = "";
}

/**
 * Remove all spaces from the current token
 */
string Lexer::cleanString(string input) {
    input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
    return input;
}

/**
 * Makes sure that all the brackets and braces in the input source file have a valid match
 */
void Lexer::checkValidBracket(char curr) {
    if (isBracket(curr)) {
        if (curr == Tokens::CHAR_OPEN_BRACKET || curr == Tokens::CHAR_OPEN_BRACE) {
            brackets.push(curr);
        }
        if (curr == Tokens::CHAR_CLOSE_BRACKET || curr == Tokens::CHAR_CLOSE_BRACE) {
            if (brackets.empty()) {
                throw invalid_argument("invalid program, brackets do not match");
            }
            if ((curr == Tokens::CHAR_CLOSE_BRACKET &&
                 brackets.top() == Tokens::CHAR_OPEN_BRACKET) ||
                (curr == Tokens::CHAR_CLOSE_BRACE && brackets.top() == Tokens::CHAR_OPEN_BRACE)) {
                brackets.pop();
            } else {
                throw invalid_argument("invalid program, brackets do not match");
            }
        }
    }
}

// special keywords

bool Lexer::isKeyword(string input) {
    return input == Tokens::KEYWORD_READ || input == Tokens::KEYWORD_PRINT ||
           input == Tokens::KEYWORD_CALL || input == Tokens::KEYWORD_WHILE ||
           input == Tokens::KEYWORD_IF || input == Tokens::KEYWORD_THEN ||
           input == Tokens::KEYWORD_PROCEDURE || input == Tokens::KEYWORD_ELSE;
}

bool Lexer::isAssignStmt(vector<string> content) {
    return find(content.begin(), content.end(), Tokens::SYMBOL_ASSIGN) != content.end();
}

bool Lexer::isStmt(vector<string> content) {
    return ((content.front() != Tokens::SYMBOL_CLOSE_BRACE &&
             content.front() != Tokens::KEYWORD_ELSE &&
             content.front() != Tokens::KEYWORD_PROCEDURE) ||
            (isAssignStmt(content)));
}

// special symbols

bool Lexer::isOperator(char input) { // logical, comparison, artihmetic and
                                     // assignment (they all overlap)
    return input == Tokens::CHAR_AMPERSAND || input == Tokens::CHAR_BAR ||
           input == Tokens::CHAR_NOT || input == Tokens::CHAR_GREATER_THAN ||
           input == Tokens::CHAR_LESS_THAN || input == Tokens::CHAR_ASSIGN ||
           input == Tokens::CHAR_PLUS || input == Tokens::CHAR_MINUS ||
           input == Tokens::CHAR_MULTIPLY || input == Tokens::CHAR_DIVIDE ||
           input == Tokens::CHAR_MODULO;
}

bool Lexer::isBracket(char input) {
    return input == Tokens::CHAR_OPEN_BRACKET || input == Tokens::CHAR_CLOSE_BRACKET ||
           input == Tokens::CHAR_OPEN_BRACE || input == Tokens::CHAR_CLOSE_BRACE;
}

bool Lexer::isSemiColon(char input) { return input == Tokens::CHAR_SEMICOLON; }

bool Lexer::isCurlyBracket(string input) {
    return input == Tokens::SYMBOL_OPEN_BRACE || input == Tokens::SYMBOL_CLOSE_BRACE;
}

bool Lexer::isSemiColon(string input) { return input == Tokens::SYMBOL_SEMICOLON; }
