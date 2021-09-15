#include "query_processor/QueryParser.h"

Reference *QueryParser::parseDeclaration(DeclPair declaration) {
    DesignEntityType type = deHelper.getType(declaration.first);
    string syn = declaration.second;
    return new Reference(type, ReferenceType::SYNONYM, syn);
}

Clause *QueryParser::parseClause(ClsTuple clause,
                                 vector<Reference *> &declList) {
    string cls = get<0>(clause);
    string ref1 = get<1>(clause);
    string ref2 = get<2>(clause);

    auto it1 = find_if(declList.begin(), declList.end(),
        [&ref1](Reference* ref) { return ref->getValue() == ref1; });
    auto it2 = find_if(declList.begin(), declList.end(),
        [&ref2](Reference* ref) { return ref->getValue() == ref2; });

    Reference *r1;
    Reference *r2;
    RefType type1;
    RefType type2;

    if (it1 != declList.end()) {
        r1 = (*it1)->copy();
        type1 = RefType::SYNONYM;
    } else {
    }

    if (it2 != declList.end()) {
        r2 = (*it2)->copy();
        type1 = RefType::SYNONYM;
    }

    type1 = getRefType(ref1); // trims quotes from string if any
    type2 = getRefType(ref2); // trims quotes from string if any

    ClauseType clsT = clsHelper.getType(cls, type1, r1->getDeType);

    checkClauseValidity(clT, );
    else {
    throw ValidityError("invalid argument type for clause");
    }
    if (rt == RefType::NUMBER) {
        if (type->second == ClauseType)
    }

    
    // check the types of the two references
    // if the reference exist in the decl list, then it must be a synonym
        // check if the synonym type matches the clause type
    // if the reference does not exist in the decl list, check its value
        // number: statement
        // quotedSynonym: variable or procedure
        // wildcard: can be anything
    // if first == number
        // FOLLOWS/FOLLOWS_T/PARENT/PARENT_T/MODIFIES_S/USES_S
    // if first == quoted
        // MODIFIES_P/USES_P
    // if first == wildcard
        // FOLLOWS/FOLLOWS_T/PARENT/PARENT_T
    
    // if second == number
        // FOLLOWS/FOLLOWS_T/PARENT/PARENT_T
    // if second == quotedConstant
        // MODIFIES_P/MODIFIES_S/USES_P/USES_S
    // if second == wildcard
        // FOLLOWS/FOLLOWS_T/PARENT/PARENT_T/MODIFIES_P/MODIFIES_S/USES_P/USES_S



    ClauseType clsT = clsHelper.getType(cls);

    vector<Reference *> x;

    auto it1 = find_if(declList.begin(), declList.end(), 
        [&ref1](Reference *ref) { return ref->getValue() == ref1; });
    auto it2 = find_if(declList.begin(), declList.end(),
        [&ref2](Reference *ref) { return ref->getValue() == ref2; });

    if (it1 != declList.end()) {
        Reference* r = (*it1)->copy();
        x.push_back(r);
    } else {
        ReferenceType refT = checkRefType(ref1);
        DesignEntityType deT = clsHelper.chooseDeType1(clsT);
        x.push_back(new Reference(deT, refT, ref1));
    }

    if (it2 != declList.end()) {
        Reference* r = (*it2)->copy();
        x.push_back(r);
    } else {
        ReferenceType refT = checkRefType(ref2);
        DesignEntityType deT = clsHelper.chooseDeType2(clsT);
        x.push_back(new Reference(deT, refT, ref2));
    }

    return new Clause(clsT, *x[0], *x[1]);
}

PatternClause* QueryParser::parsePattern(PatTuple pattern, vector<Reference*>& declList) {
    string identity = get<0>(pattern);
    vector<PatArg> patArgs = get<1>(pattern);
    string LHS = patArgs[0];
    string RHS = patArgs[1];
    
    Reference* r;
    Reference* r1;
    auto it = find_if(declList.begin(), declList.end(),
        [&identity](Reference* ref) { return ref->getValue() == identity; });
    auto it1 = find_if(declList.begin(), declList.end(),
        [&LHS](Reference* ref) { return ref->getValue() == LHS; });

    if (it != declList.end()) {
        r = (*it)->copy();
    } else {
        throw ValidityError("undeclared synonym");
    }

    if (it1 != declList.end()) {
        r1 = (*it1)->copy();
    } else {
        ReferenceType refT = checkRefType(LHS);
        DesignEntityType deT = DesignEntityType::VARIABLE;
        r1 = new Reference(deT, refT, LHS);
    }
    
    return new PatternClause(*r, *r1, RHS);
}

// helper methods
RefType QueryParser::getRefType(string& val) {
    if (isWildcard(val)) {
        return RefType::WILDCARD;
    } else if (isInteger(val)) {
        return RefType::NUMBER;
    } else if (isQuoted(val)) {
        return RefType::QUOTED;
    }
    throw ValidityError("undeclared synonym");
}

ReferenceType QueryParser::checkRefType(string &val) {
    if (isWildcard(val)) {
        return ReferenceType::WILDCARD;
    } else if (isInteger(val)) {
        return ReferenceType::CONSTANT;
    } else if (isQuoted(val)) {
        return ReferenceType::CONSTANT;
    }
    throw ValidityError("undeclared synonym");
}

bool QueryParser::isInteger(string val) {
    return all_of(val.begin(), val.end(), isdigit);
}

// " "
bool QueryParser::isQuoted(string &val) {
    int c = count(val.begin(), val.end(), '"');

    if (c != 2) {
        return false;
    }
    
    size_t pos1 = val.find_first_of('"');
    size_t pos2 = val.find_last_of('"');

    if (pos1 == 0 && pos2 == val.size() - 1) {
        val = val.substr(pos1, val.size() - 2);
        return true;
    }
    return false;
}

// _
bool QueryParser::isWildcard(string val) {
    return val == "_";
}
