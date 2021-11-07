#include "query_processor/parser/PatternParser.h"

void PatternParser::clear() {
    this->declList.clear();
    delete this->r1, this->r2;
    this->r1 = nullptr;
    this->r2 = nullptr;
    this->ref1 = "";
    this->ref2 = "";
    this->tokens.clear();
}

/**
 * Parses a PatTuple into a Clause object.
 * @param patTuple Tuple of <stmt, var, vector<token>>.
 * @return Clause object of type PATTERN.
 */
Clause *PatternParser::parsePt(PatTuple patTuple) {
    this->ref1 = get<0>(patTuple);
    this->ref2 = get<1>(patTuple);
    this->tokens = get<2>(patTuple);

    r1 = getReferenceIfDeclared(this->ref1);
    if (r1 == nullptr) {
        throw ValidityError("QP-ERROR: undeclared pattern synonym");
    }
    r1 = r1->copy();

    r2 = parseEntRef(this->ref2, DesignEntityType::VARIABLE);
    if (r2 == nullptr) {
        throw ValidityError("QP-ERROR: invalid pattern clause argument");
    }

    if (isAssignPatternClause()) {
        return parseAssign();
    } else if (isWhilePatternClause() || isIfPatternClause()) {
        return new Clause(ClauseType::PATTERN, *r1, *r2);
    } else {
        throw ValidityError("QP-ERROR: invalid pattern clause");
    }
}

Clause *PatternParser::parseAssign() {
    vector<string> tokens = this->tokens;

    if (isWildcardPattern(tokens)) {
        return new Clause(*r1, *r2, {}, true);
    }

    bool isExactMatch = isExactPattern(tokens);
    tokens = parsePatternTokens();
    return new Clause(*r1, *r2, tokens, isExactMatch);
}

vector<PatToken> PatternParser::parsePatternTokens() {
    vector<PatToken> validatedTokens;

    for (auto t : this->tokens) {
        if (!ParserUtil::isWildcard(t) && !ParserUtil::isQuote(t)) {
            validatedTokens.push_back(t);
        }
    }

    return validatedTokens;
}

bool PatternParser::isAssignPatternClause() {
    return r1->getDeType() == DesignEntityType::ASSIGN && !this->tokens.empty();
}

bool PatternParser::isWhilePatternClause() {
    return r1->getDeType() == DesignEntityType::WHILE &&
           this->tokens.size() == PATARG_SIZE_ONE &&
           ParserUtil::isWildcard(this->tokens.at(0));
}

bool PatternParser::isIfPatternClause() {
    return r1->getDeType() == DesignEntityType::IF &&
           this->tokens.size() == PATARG_SIZE_TWO &&
           ParserUtil::isWildcard(this->tokens.at(0)) &&
           ParserUtil::isWildcard(this->tokens.at(1));
}

bool PatternParser::isExactPattern(vector<PatToken> pattern) {
    if (isQuotedPattern(pattern)) {
        return true;
    } else if (isUnderscoreQuotedPattern(pattern)) {
        return false;
    }
    throw ValidityError("invalid pattern string");
}

bool PatternParser::isWildcardPattern(vector<PatToken> pattern) {
    return pattern.size() == PATARG_SIZE_ONE &&
           ParserUtil::isWildcard(pattern.at(0));
}

bool PatternParser::isQuotedPattern(vector<PatToken> pattern) {
    return ParserUtil::isQuote(pattern.at(0)) &&
           ParserUtil::isQuote(pattern.at(pattern.size() - 1));
}

bool PatternParser::isUnderscoreQuotedPattern(vector<PatToken> pattern) {
    return ParserUtil::isWildcard(pattern.at(0)) &&
           ParserUtil::isWildcard(pattern.at(pattern.size() - 1)) &&
           ParserUtil::isQuote(pattern.at(1)) &&
           ParserUtil::isQuote(pattern.at(pattern.size() - 2));
}
