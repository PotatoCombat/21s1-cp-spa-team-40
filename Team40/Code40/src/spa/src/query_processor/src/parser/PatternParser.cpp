#include "query_processor/parser/PatternParser.h"

PatternParser::PatternParser() {
    this->ident = "";
    this->var = "";
    this->tokens.clear();
}

void PatternParser::initReferences(vector<Reference *> &declList) {
    this->declList = declList;
}

void PatternParser::clear() {
    this->declList.clear();
    this->ident = "";
    this->var = "";
    this->tokens.clear();
}

/**
 * Parse a PatTuple into a Clause object.
 * @param patTuple Tuple of <stmt, var, vector<token>>.
 * @return Clause object of type PATTERN.
 */
Clause *PatternParser::parse(PatTuple patTuple) {
    this->ident = get<0>(patTuple);
    this->var = get<1>(patTuple);
    this->tokens = get<2>(patTuple);

    Reference *identity = getReferenceIfDeclared(this->ident);

    if (identity == nullptr) {
        throw ValidityError("undeclared pattern synonym");
    }

    identity = identity->copy();

    if (isAssignPatternClause(identity)) {
        return parseAssign(identity);
    } else if (isWhilePatternClause(identity)) {
        return parseWhile(identity);
    } else if (isIfPatternClause(identity)) {
        return parseIf(identity);
    } else {
        delete identity;
        throw ValidityError("invalid pattern type");
    }
}

Clause *PatternParser::parseAssign(Reference *identity) {
    string var = this->var;
    vector<string> tokens = this->tokens;
    Reference *ref = parseValidVariable(var);

    if (isWildcardPattern(tokens)) {
        return new Clause(*identity, *ref, vector<string>{}, true);
    }

    bool isExactMatch = isExactPattern(tokens);
    tokens = parsePatternTokens(tokens);
    return new Clause(*identity, *ref, tokens, isExactMatch);
}

Clause *PatternParser::parseWhile(Reference *identity) {
    string var = this->var;
    vector<string> tokens = this->tokens;
    Reference *ref = parseValidVariable(var);

    if (!ParserUtil::isWildcard(tokens.at(0))) {
        throw ValidityError("while clause 2nd argument should be _");
    }
    return new Clause(*identity, *ref);
}

Clause *PatternParser::parseIf(Reference *identity) {
    string var = this->var;
    vector<string> tokens = this->tokens;
    Reference *ref = parseValidVariable(var);

    if (!ParserUtil::isWildcard(tokens.at(0)) ||
        !ParserUtil::isWildcard(tokens.at(1))) {
        throw ValidityError("if clause 2nd & 3rd arguments should be _");
    }
    return new Clause(*identity, *ref);
}

Reference *PatternParser::parseValidVariable(string var) {
    Reference *ref = getReferenceIfDeclared(var);

    if (ref != nullptr) {
        if (ref->getDeType() != DesignEntityType::VARIABLE) {
            throw ValidityError("invalid reference");
        }
        return ref->copy();
    }

    DesignEntityType deT = DesignEntityType::VARIABLE;
    ReferenceType refT;
    if (ParserUtil::isWildcard(var)) {
        refT = ReferenceType::WILDCARD;
    } else if (ParserUtil::isQuoted(var)) {
        refT = ReferenceType::CONSTANT;
        var = var.substr(1, var.size() - 2);
    } else {
        throw ValidityError("invalid reference");
    }
    return new Reference(deT, refT, var);
}

vector<PatToken> PatternParser::parsePatternTokens(vector<PatToken> tokens) {
    vector<PatToken> validatedTokens;
    size_t bracketCount = 0;
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
            } else if (ParserUtil::isValidName(t) ||
                        ParserUtil::isInteger(t)) {
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

    return validatedTokens;
}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *PatternParser::getReferenceIfDeclared(string syn) {
    auto it = find_if(declList.begin(), declList.end(), [&syn](Reference *ref) {
        return ref->getValue() == syn;
    });

    if (it != declList.end()) {
        return *it;
    }
    return nullptr;
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
    return pattern.size() >= 2 && 
           ParserUtil::isQuote(pattern.at(0)) &&
           ParserUtil::isQuote(pattern.at(pattern.size() - 1)) &&
           count(pattern.begin(), pattern.end(), ParserUtil::isQuote) == 2;
}

bool PatternParser::isUnderscoreQuotedPattern(vector<PatToken> pattern) {
    return pattern.size() >= 4 && 
           ParserUtil::isWildcard(pattern.at(0)) &&
           ParserUtil::isWildcard(pattern.at(pattern.size() - 1)) &&
           ParserUtil::isQuote(pattern.at(1)) &&
           ParserUtil::isQuote(pattern.at(pattern.size() - 2)) &&
           count(pattern.begin(), pattern.end(), ParserUtil::isWildcard) == 2 &&
           count(pattern.begin(), pattern.end(), ParserUtil::isQuote) == 2;
}

bool PatternParser::isOperator(string token) {
    string OPERATOR_SET = "+-*/%";
    return OPERATOR_SET.find(token) != string::npos;
}

bool PatternParser::isLBracket(string token) { return token == "("; }

bool PatternParser::isRBracket(string token) { return token == ")"; }
