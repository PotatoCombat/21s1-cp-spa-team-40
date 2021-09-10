#include "QueryParser.h"

QueryParser::QueryParser() = default;

Reference QueryParser::parseDeclaration(pair<string, string> declaration) {
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
        throw "error";
    }
    return Reference(type, ReferenceType::SYNONYM, syn);
}

Clause QueryParser::parseSuchThatClause(tuple<string, string, string> clause) {
    string relation = get<0>(clause);
    string ref1 = get<1>(clause); // need information about type from declaration parser monkaS
    string ref2 = get<2>(clause); // need information about type from declaration parser monkaS
    Reference r1 = Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, ref1);
    Reference r2 = Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, ref2);

    ClauseType type;

    if (relation == "Follows") {
        type = ClauseType::FOLLOWS;
    } else if (relation == "Follows*") {
        type = ClauseType::FOLLOWS_T;
    } else {
        throw "Error";
    }
    return Clause(type, r1, r2);
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

