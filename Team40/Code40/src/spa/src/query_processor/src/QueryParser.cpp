#include "query_processor/QueryParser.h"

void QueryParser::clear() {
    this->stParser.clear();
    this->ptParser.clear();
    this->deleteDeclarations();
}

void QueryParser::deleteDeclarations() {
    for (int i = 0; i < this->declList.size(); ++i) {
        delete this->declList[i];
    }
    this->declList.clear();
}

void QueryParser::parseDeclarations(vector<DeclPair> declPairs) {
    for (auto x : declPairs) {
        DesignEntityType type = deHelper.valueToDesType(x.first);
        string syn = x.second;
        Reference *ref = new Reference(type, ReferenceType::SYNONYM, syn);
        declList.push_back(ref);
    }

    stParser.initReferences(declList);
    ptParser.initReferences(declList);
}

Reference *QueryParser::parseReturnSynonym(string syn, bool &found) {
    for (auto x : declList) {
        if (syn == x->getValue()) {
            found = true;
            return x->copy();
        }
    }
    return nullptr;
}

Clause *QueryParser::parseSuchThatClause(ClsTuple clsTuple) {
    return stParser.parse(clsTuple);
}

PatternClause *QueryParser::parsePatternClause(PatTuple patTuple) {
    return ptParser.parse(patTuple);
}
