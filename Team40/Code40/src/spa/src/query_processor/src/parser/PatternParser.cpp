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
 * Parse a PatTuple into a Clause object.
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
    int bracketCount = 0;
    bool isWord = true;

    for (auto t : tokens) {
        if (ParserUtil::isWildcard(t) || ParserUtil::isQuote(t)) {
            continue;
        }
        if (isWord) {
            if (isLBracket(t)) {
                isWord = true;
                bracketCount += 1;
                validatedTokens.push_back(t);
            } else if (ParserUtil::isValidName(t) || ParserUtil::isInteger(t)) {
                isWord = false;
                validatedTokens.push_back(t);
            } else {
                throw ValidityError("Invalid pattern string");
            }
        } else {
            if (isOperator(t)) {
                isWord = true;
                validatedTokens.push_back(t);
            } else if (isRBracket(t)) {
                isWord = false;
                bracketCount -= 1;
                validatedTokens.push_back(t);
            } else {
                throw ValidityError("Invalid pattern string");
            }
        }
    }

    if (bracketCount != 0 || isWord == true) {
        throw ValidityError("Invalid pattern string");
    }

    return validatedTokens;
}

bool PatternParser::isAssignPatternClause() {
    return r1->getDeType() == DesignEntityType::ASSIGN &&
           this->tokens.size() > 0;
}

bool PatternParser::isWhilePatternClause() {
    return r1->getDeType() == DesignEntityType::WHILE &&
           this->tokens.size() == 1;
}

bool PatternParser::isIfPatternClause() {
    return r1->getDeType() == DesignEntityType::IF &&
           this->tokens.size() == 2;
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
    return pattern.size() == 1 && ParserUtil::isWildcard(pattern.at(0));
}

bool PatternParser::isQuotedPattern(vector<PatToken> pattern) {
    int c = countOccurences(pattern, "\"");
    return pattern.size() >= 2 && c == 2 &&
           ParserUtil::isQuote(pattern.at(0)) &&
           ParserUtil::isQuote(pattern.at(pattern.size() - 1));
}

bool PatternParser::isUnderscoreQuotedPattern(vector<PatToken> pattern) {
    int c1 = countOccurences(pattern, "_");
    int c2 = countOccurences(pattern, "\"");
    return pattern.size() >= 4 && c1 == 2 && c2 == 2 &&
           ParserUtil::isWildcard(pattern.at(0)) &&
           ParserUtil::isWildcard(pattern.at(pattern.size() - 1)) &&
           ParserUtil::isQuote(pattern.at(1)) &&
           ParserUtil::isQuote(pattern.at(pattern.size() - 2));
}

bool PatternParser::isOperator(string token) {
    string OPERATOR_SET = "+-*/%";
    return OPERATOR_SET.find(token) != string::npos;
}

bool PatternParser::isLBracket(string token) { return token == "("; }

bool PatternParser::isRBracket(string token) { return token == ")"; }

int PatternParser::countOccurences(vector<string> vec, string token) {
    int c = 0;
    for (auto x : vec) {
        if (token == x) {
            c += 1;
        }
    }
    return c;
}
