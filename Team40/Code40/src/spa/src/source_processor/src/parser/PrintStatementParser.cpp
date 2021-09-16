#include "source_processor/parser/PrintStatementParser.h"
#include <algorithm>

PrintStatementParser::PrintStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    stmt = new Statement(index, StatementType::PRINT);
};

Statement *PrintStatementParser::parsePrintStatement() {
    vector<string>::iterator printItr =
        find(content.begin(), content.end(), "print");

    string var_name = *next(printItr);
    if (!isValidName(var_name)) {
        throw("invalid variable name");
    }
    // print: 'print' var_name';'
    if (*next(next(printItr)) != ";") {
        throw("invalid print statement");
    }
    auto variable = new Variable(var_name);
    stmt->setVariable(variable);
    return stmt;
}

bool PrintStatementParser::isValidName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // procedure names and variables are strings of letters, and digits,
    // starting with a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}
