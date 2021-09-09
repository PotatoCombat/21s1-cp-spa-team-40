#include "QueryParser.h"

QueryParser::QueryParser() = default;

Reference QueryParser::parseDeclaration(DeclTuple declaration) {
    DesignEntityType type = deTypeHelper.getType(declaration.first);
    string syn = declaration.second;
    return Reference(type, ReferenceType::SYNONYM, syn);
}

Relation QueryParser::parseRelation(RelTuple clause,
                                          vector<Reference> &declList) {
    string rel = get<0>(clause);
    string ref1 = get<1>(clause);
    string ref2 = get<2>(clause);

    RelationType type = relTypeHelper.getType(rel);

    Reference r1;
    Reference r2;

    auto it1 = find_if(declList.begin(), declList.end(), 
        [&ref1](Reference& ref) { return ref.getValue() == ref1; });
    auto it2 = find_if(declList.begin(), declList.end(),
        [&ref2](Reference& ref) { return ref.getValue() == ref2; });

    if (it1 != declList.end()) {
        r1 = *it1;
    } else { // either a undeclared synonym, number, quoted expression, or _
        r1 = Reference(); // TODO
    }

    if (it2 != declList.end()) {
        r1 = *it2;
    }
    else { // either a undeclared synonym, number, quoted expression, or _
        r1 = Reference(); // TODO
    }

    return Relation(type, &r1, &r2);
}

// PatternClause QueryParser::parsePatternClause(tuple<string, string, string>
// clause) {
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
