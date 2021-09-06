#include "QueryParser.h"
#include <relation/FollowsRelation.h>
#include <relation/FollowsStarRelation.h>
#include <relation/ModifiesStatementRelation.h>
#include <relation/ParentRelation.h>
#include <relation/ParentStarRelation.h>
#include <relation/UsesStatementRelation.h>

QueryParser::QueryParser() = default;

Declaration QueryParser::parseDeclaration(pair<string, string> declaration) {
    string designEntity = declaration.first;
    string syn = declaration.second;
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
    return Declaration(syn, type);
}

SuchThatClause QueryParser::parseSuchThatClause(tuple<string, string, string> clause) {
    string relation = get<0>(clause);
    string ref1 = get<1>(clause);
    string ref2 = get<2>(clause);

    //Relation *rel;

    if (relation == "Follows") {
        FollowsRelation r(ref1, ref2);
        return SuchThatClause(r);
    } else if (relation == "Follows*") {
        FollowsStarRelation r(ref1, ref2);
        return SuchThatClause(r);
    //} else if (relation == "Parent") {
    //    ParentRelation r(ref1, ref2);
    //    rel = &r;
    //} else if (relation == "Parent*") {
    //    ParentStarRelation r(ref1, ref2);
    //    rel = &r;
    //} else if (relation == "Modifies") {
    //    ModifiesStatementRelation r(ref1, ref2);
    //    rel = &r;
    //} else if (relation == "Uses") {
    //    UsesStatementRelation r(ref1, ref2);
    //    rel = &r;
    } else {
        Relation r(ref1, ref2, RelationType::UNKNOWN);
        return SuchThatClause(r);
    }
}

//PatternClause QueryParser::parsePatternClause(tuple<string, string, string> clause) {
//    vector<PatternClause> cl;
//    for (auto &it : clause) {
//        // parse by syn '(' Ref1 ',' Ref2 ')'
//        vector<string> tokens = tokenizeClause(it);
//        string syn = tokens[0];
//        EntityReference ref = EntityReference(tokens[1]);
//        Expression expr = Expression(tokens[2]);
//
//        cl.push_back(PatternClause(syn, ref, expr));
//    }
//    return cl;
//}

