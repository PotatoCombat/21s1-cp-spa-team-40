#include "QueryParser.h"

QueryParser::QueryParser() {}

SelectClause QueryParser::parseQuery(
    vector<string> declarations,
    tuple<string, vector<string>, vector<string>> selectClause) {
    // for every declaration string
    // parse into declaration object
    // should have a vector of declarations
    vector<Declaration> decl = parseDeclarations(declarations);

    // parse the selectClause into returnEntity and clauses
    string returnEntity = get<0>(selectClause);

    vector<SuchThatClause> suchThatCl =
        parseSuchThatClause(get<1>(selectClause));
    vector<PatternClause> patternCl = parsePatternClause(get<2>(selectClause));

    return SelectClause(returnEntity, decl, suchThatCl, patternCl);
}

vector<Declaration>
QueryParser::parseDeclarations(vector<string> declarations) {
    vector<Declaration> decl;
    for (auto &it : declarations) {
        size_t found =
            it.find(" "); // find whitespace to separate designEntity and syn
        if (found != string::npos) {
            string designEntity = it.substr(0, found);
            string syn = it.substr(found + 1);
            DesignEntityType type;
            if (designEntity == "stmt") {
                type = DesignEntityType::STMT;
            } else if (designEntity == "assign") {
                type = DesignEntityType::ASSIGN;
            } else if (designEntity == "variable") {
                type = DesignEntityType::VARIABLE;
            } else if (designEntity == "constant") {
                type = DesignEntityType::CONSTANT;
            } else if (designEntity == "procedure") {
                type = DesignEntityType::PROCEDURE;
            } else if (designEntity == "read") {
                type = DesignEntityType::READ;
            } else if (designEntity == "print") {
                type = DesignEntityType::PRINT;
            } else if (designEntity == "while") {
                type = DesignEntityType::WHILE;
            } else if (designEntity == "if") {
                type = DesignEntityType::IF;
            } else if (designEntity == "call") {
                type = DesignEntityType::CALL;
            } else {
                // invalid entity declaration
            }
            decl.push_back(Declaration(syn, type));
        }
    }
    return decl;
}

vector<SuchThatClause> QueryParser::parseSuchThatClause(vector<string> clause) {
    vector<SuchThatClause> cl;
    for (auto &it : clause) {
        // parse by Relation '(' Ref1 ',' Ref2 ')'
    }
    return cl;
}

vector<PatternClause> QueryParser::parsePatternClause(vector<string> clause) {
    vector<PatternClause> cl;
    for (auto &it : clause) {
        // parse by syn '(' Ref1 ',' Ref2 ')'
    }
    return cl;
}
