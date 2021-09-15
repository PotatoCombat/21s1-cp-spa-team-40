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
        throw PreprocessorException("undeclared synonym");
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

ReferenceType QueryParser::checkRefType(string &val) {
    if (isWildcard(val)) {
        return ReferenceType::WILDCARD;
    } else if (isInteger(val)) {
        return ReferenceType::CONSTANT;
    } else if (isNamedSynonym(val)) {
        return ReferenceType::CONSTANT;
    }
    throw PreprocessorException("undeclared synonym");
}

bool QueryParser::isInteger(string val) {
    return all_of(val.begin(), val.end(), isdigit);
}

// " "
bool QueryParser::isNamedSynonym(string &val) {
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