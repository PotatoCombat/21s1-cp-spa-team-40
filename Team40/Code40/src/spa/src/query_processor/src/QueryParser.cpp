#include "QueryParser.h"

Reference QueryParser::parseDeclaration(DeclPair declaration) {
    DesignEntityType type = deHelper.getType(declaration.first);
    string syn = declaration.second;
    return Reference(type, ReferenceType::SYNONYM, syn);
}

Relation QueryParser::parseRelation(RelTuple clause,
                                    vector<Reference> &declList) {
    string rel = get<0>(clause);
    string ref1 = get<1>(clause);
    string ref2 = get<2>(clause);

    RelationType relT = relHelper.getType(rel);

    vector<Reference> x;

    auto it1 = find_if(declList.begin(), declList.end(), 
        [&ref1](Reference& ref) { return ref.getValue() == ref1; });
    auto it2 = find_if(declList.begin(), declList.end(),
        [&ref2](Reference& ref) { return ref.getValue() == ref2; });

    if (it1 != declList.end()) {
        x.push_back(*it1);
    } else {
        ReferenceType refT = checkRefType(ref1);
        DesignEntityType deT = relHelper.chooseDeType1(relT);
        x.push_back(Reference(deT, refT, ref1));
    }

    if (it2 != declList.end()) {
        x.push_back(*it2);
    } else {
        ReferenceType refT = checkRefType(ref2);
        DesignEntityType deT = relHelper.chooseDeType2(relT);
        x.push_back(Reference(deT, refT, ref2));
    }

    return Relation(relT, &x[0], &x[1]);
}

// helper methods

ReferenceType QueryParser::checkRefType(string val) {
    if (isWildcard(val)) {
        return ReferenceType::WILDCARD;
    } else if (isInteger(val)) {
        return ReferenceType::CONSTANT;
    } else if (isNamedSynonym(val)) {
        return ReferenceType::CONSTANT;
    }
    return ReferenceType::SYNONYM;
}

bool QueryParser::isInteger(string val) {
    return all_of(val.begin(), val.end(), isdigit);
}

// " "
bool QueryParser::isNamedSynonym(string val) {
    int c = count(val.begin(), val.end(), '"');

    if (c != 2) {
        return false;
    }
    
    size_t pos1 = val.find_first_of('"');
    size_t pos2 = val.find_last_of('"');

    return pos1 == 0 && pos2 == val.size() - 1;
}

// _
bool QueryParser::isWildcard(string val) {
    return val == "_";
}