#include "query_processor/parser/PatternParser.h"

void PatternParser::clear() {
    this->declList.clear();
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

    Reference *stmt = getReferenceIfDeclared(this->ref1);
    if (stmt == nullptr) {
        throw ValidityError("undeclared pattern synonym");
    }

    Reference *var = parseEntRef(this->ref2, DesignEntityType::VARIABLE);
    if (var == nullptr) {
        throw ValidityError("invalid clause argument");
    }
    stmt = stmt->copy();

    if (isAssignPatternClause(stmt)) {
        return parseAssign(stmt, var);
    } else if (isWhilePatternClause(stmt)) {
        return parseWhile(stmt, var);
    } else if (isIfPatternClause(stmt)) {
        return parseIf(stmt, var);
    } else {
        delete stmt, var;
        throw ValidityError("invalid pattern type");
    }
}

Clause *PatternParser::parseAssign(Reference *stmt, Reference *var) {
    vector<string> tokens = this->tokens;

    if (isWildcardPattern(tokens)) {
        return new Clause(*stmt, *var, vector<string>{}, true);
    }

    bool isExactMatch = isExactPattern(tokens);
    tokens = parsePatternTokens(tokens);
    return new Clause(*stmt, *var, tokens, isExactMatch);
}

Clause *PatternParser::parseWhile(Reference *stmt, Reference *var) {
    vector<string> tokens = this->tokens;

    if (!ParserUtil::isWildcard(tokens.at(0))) {
        throw ValidityError("while clause 2nd argument should be _");
    }
    return new Clause(ClauseType::PATTERN, *stmt, *var);
}

Clause *PatternParser::parseIf(Reference *stmt, Reference *var) {
    vector<string> tokens = this->tokens;

    if (!ParserUtil::isWildcard(tokens.at(0)) ||
        !ParserUtil::isWildcard(tokens.at(1))) {
        throw ValidityError("if clause 2nd & 3rd arguments should be _");
    }
    return new Clause(ClauseType::PATTERN, *stmt, *var);
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

bool PatternParser::isAssignPatternClause(Reference *identity) {
    return identity->getDeType() == DesignEntityType::ASSIGN &&
           this->tokens.size() > 0;
}

bool PatternParser::isWhilePatternClause(Reference *identity) {
    return identity->getDeType() == DesignEntityType::WHILE &&
           this->tokens.size() == 1;
}

bool PatternParser::isIfPatternClause(Reference *identity) {
    return identity->getDeType() == DesignEntityType::IF &&
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
