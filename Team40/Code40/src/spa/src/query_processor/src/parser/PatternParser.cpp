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
        throw ValidityError("undeclared pattern synonym");
    }
    r1 = r1->copy();

    r2 = parseEntRef(this->ref2, DesignEntityType::VARIABLE);
    if (r2 == nullptr) {
        throw ValidityError("invalid clause argument");
    }

    if (isAssignPatternClause()) {
        return parseAssign();
    } else if (isWhilePatternClause()) {
        return parseWhile();
    } else if (isIfPatternClause()) {
        return parseIf();
    } else {
        throw ValidityError("invalid pattern type");
    }
}

Clause *PatternParser::parseAssign() {
    vector<string> tokens = this->tokens;

    if (isWildcardPattern(tokens)) {
        return new Clause(*r1, *r2, vector<string>{}, true);
    }

    bool isExactMatch = isExactPattern(tokens);
    tokens = parsePatternTokens(tokens);
    return new Clause(*r1, *r2, tokens, isExactMatch);
}

Clause *PatternParser::parseWhile() {
    vector<string> tokens = this->tokens;

    if (!ParserUtil::isWildcard(tokens.at(0))) {
        throw ValidityError("while clause 2nd argument should be _");
    }
    return new Clause(ClauseType::PATTERN, *r1, *r2);
}

Clause *PatternParser::parseIf() {
    vector<string> tokens = this->tokens;

    if (!ParserUtil::isWildcard(tokens.at(0)) ||
        !ParserUtil::isWildcard(tokens.at(1))) {
        throw ValidityError("if clause 2nd & 3rd arguments should be _");
    }
    return new Clause(ClauseType::PATTERN, *r1, *r2);
}

vector<PatToken> PatternParser::parsePatternTokens(vector<PatToken> tokens) {
    vector<PatToken> validatedTokens;

    for (auto t : tokens) {
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
           this->tokens.size() == PATARG_SIZE_ONE;
}

bool PatternParser::isIfPatternClause() {
    return r1->getDeType() == DesignEntityType::IF &&
           this->tokens.size() == PATARG_SIZE_TWO;
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
