#include "QueryParser.h"
#include <relation/FollowsRelation.h>
#include <relation/FollowsStarRelation.h>
#include <relation/ModifiesStatementRelation.h>
#include <relation/ParentRelation.h>
#include <relation/ParentStarRelation.h>
#include <relation/UsesStatementRelation.h>

QueryParser::QueryParser() {}

SelectClause QueryParser::parseQuery(vector<string> declarations,
    tuple<string, vector<string>, vector<string>> clause) {
    vector<Declaration> decl = parseDeclarations(declarations);

    // parse the selectClause into returnEntity and clauses
    string returnEntity = get<0>(clause);

    vector<SuchThatClause> suchThatCl =
        parseSuchThatClause(get<1>(clause));
    vector<PatternClause> patternCl = parsePatternClause(get<2>(clause));

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
                type = DesignEntityType::UNKNOWN;
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
        vector<string> tokens = tokenizeClause(it);
        string relation = tokens[0];
        Reference ref1 = Reference(tokens[1]);
        Reference ref2 = Reference(tokens[2]);
        Relation *rel;
        if (relation == "Follows") {
            FollowsRelation r(ref1, ref2);
            rel = &r;
        } else if (relation == "Follows*") {
            FollowsStarRelation r(ref1, ref2);
            rel = &r;
        } else if (relation == "Parent") {
            ParentRelation r(ref1, ref2);
            rel = &r;
        } else if (relation == "Parent*") {
            ParentStarRelation r(ref1, ref2);
            rel = &r;
        } else if (relation == "Modifies") {
            ModifiesStatementRelation r(ref1, ref2);
            rel = &r;
        } else if (relation == "Uses") {
            UsesStatementRelation r(ref1, ref2);
            rel = &r;
        } else {
            Relation r(ref1, ref2);
            rel = &r;
        }
        cl.push_back(SuchThatClause(*rel));
    }
    return cl;
}

vector<PatternClause> QueryParser::parsePatternClause(vector<string> clause) {
    vector<PatternClause> cl;
    for (auto &it : clause) {
        // parse by syn '(' Ref1 ',' Ref2 ')'
        vector<string> tokens = tokenizeClause(it);
        string syn = tokens[0];
        EntityReference ref = EntityReference(tokens[1]);
        Expression expr = Expression(tokens[2]);

        cl.push_back(PatternClause(syn, ref, expr));
    }
    return cl;
}

vector<string> QueryParser::tokenizeClause(string clause) {
    char front_bracket = '(';
    char back_bracket = ')';
    char comma = ',';
    size_t first = clause.find(front_bracket);
    size_t second = clause.find(comma);
    size_t third = clause.find(back_bracket);
    string relation;
    string ref1;
    string ref2;
    if (first != string::npos && second != string::npos &&
        third != string::npos) {
        relation = clause.substr(0, first);
        ref1 = clause.substr(first + 1, second - first - 1);
        ref2 = clause.substr(second + 1, third - second - 1);
    } else {
        return vector<string>();
    }
    return vector<string>{relation, ref1, ref2};
}
