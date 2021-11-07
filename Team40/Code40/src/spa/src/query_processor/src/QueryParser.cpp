#include "query_processor/QueryParser.h"

void QueryParser::clear() {
    this->stParser.clear();
    this->ptParser.clear();
    this->wtParser.clear();
    this->clearDeclarations();
}

void QueryParser::clearDeclarations() {
    for (size_t i = 0; i < this->declList.size(); ++i) {
        delete this->declList[i];
    }
    this->declList.clear();
}

/**
 * Parses declared synonyms and stores them inside parser.
 * @param declPairs Pairs of declarations as <type, synonym>.
 */
void QueryParser::parseDeclarations(vector<DeclPair> declPairs) {
    for (auto x : declPairs) {
        DesignEntityType deType = deHelper.valueToDesType(x.first);
        ReferenceType refType = ReferenceType::SYNONYM;
        string syn = x.second;
        Reference *ref = getReferenceIfDeclared(syn);
        if (ref != nullptr) {
            throw ValidityError("QP-ERROR: synonym has been declared");
        }
        ReferenceAttribute attr = deHelper.typeToDefaultAttr(deType);
        ref = new Reference(deType, refType, syn, attr);
        declList.push_back(ref);
    }

    stParser.initReferences(declList);
    ptParser.initReferences(declList);
    wtParser.initReferences(declList);
}

/**
 * Parses return reference by checking if it is in the declaration list.
 * @param ref Return reference to parse.
 * @return Reference object.
 * @exception ValidityError if return reference is not declared.
 */
Reference *QueryParser::parseReturnSynonym(string ref) {
    string syn = ref;
    string attrStr = "";

    bool isAttrRef = ParserUtil::isAttrRef(ref);
    if (isAttrRef) {
        AttrRef attrRef = ParserUtil::splitAttrRef(ref);
        syn = attrRef.first;
        attrStr = attrRef.second;
    }

    Reference *x = getReferenceIfDeclared(syn);
    if (x == nullptr) {
        throw ValidityError("QP-ERROR: return synonym is undeclared");
    }

    DesignEntityType deType = x->getDeType();
    ReferenceType refType = x->getRefType();
    ReferenceAttribute attr = x->getAttr();
    if (isAttrRef) {
        attr = ParserUtil::parseValidAttr(deType, attrStr);
    }
    if (deType == DesignEntityType::PROG_LINE) {
        deType = DesignEntityType::STMT;
    }
    return new Reference(deType, refType, syn, attr);
}

/**
 * Parses a `such that` clause.
 * @param clsTuple Tuple as <type, ref1, ref2>.
 * @return Clause object.
 */
Clause *QueryParser::parseSuchThatClause(ClsTuple clsTuple) {
    return stParser.parse(clsTuple);
}

/**
 * Parses a `pattern` clause.
 * @param patTuple as <stmt, var, vector<token>>.
 * @return Clause object.
 */
Clause *QueryParser::parsePatternClause(PatTuple patTuple) {
    return ptParser.parse(patTuple);
}

/**
 * Parses a `with` clause.
 * @param withPair as <ref, ref>.
 * @return Clause object.
 */
Clause *QueryParser::parseWithClause(WithPair withPair) {
    return wtParser.parse(withPair);
}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *QueryParser::getReferenceIfDeclared(string syn) {
    return ParserUtil::getReferenceFromList(declList, syn);
}
