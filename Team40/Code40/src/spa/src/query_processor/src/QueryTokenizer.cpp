#include "query_processor/QueryTokenizer.h"

/**
 * Separates a query string into declaration string and select clause string.
 * @param input The query string.
 * @return Pair of strings where first is the declaration string and second is
 * the select clause string.
 */
pair<string, string> QueryTokenizer::separateQueryString(string input) {
    size_t lastSemicolon = input.rfind(SEMICOLON);

    if (lastSemicolon == string::npos) {
        return make_pair(EMPTY_STR, trim(input));
    }

    string decl = trim(input.substr(0, lastSemicolon + 1));
    string slct = trim(input.substr(lastSemicolon + 1));
    return make_pair(decl, slct);
}

/**
 * Tokenizes the entire declaration string into individual declaration pairs and
 * adds them to a vector.
 * @param input The declarations string (assumes at least one declaration).
 * @param &decls Vector of declaration pairs.
 */
void QueryTokenizer::tokenizeDeclarations(string input,
                                          vector<DeclPair> &decls) {
    size_t nextSemicolonPos = input.find(SEMICOLON);
    while (nextSemicolonPos != string::npos) {
        size_t firstWhitespacePos = findNextWhitespace(input, 0);
        string type = input.substr(0, firstWhitespacePos);

        validateDeType(type);

        // synoyms lie between whitespace and semicolon
        string synonyms = trim(input.substr(
            firstWhitespacePos, nextSemicolonPos - firstWhitespacePos));
        vector<string> syns = tokenizeCommaSeparatedValues(synonyms);

        // add pairs to the vector
        for (auto s : syns) {
            validateName(s);
            decls.push_back(make_pair(type, s));
        }

        // prepare for next iteration of while loop
        input = trimL(input.substr(nextSemicolonPos + 1));
        nextSemicolonPos = input.find(SEMICOLON);
    }
    return;
}

/**
 * Extracts the return synonym from the select clause and passes the remaining
 * of the clause to a string.
 * @param input The select clause string (assumes non-empty string).
 * @param &returnSynonym The return synonym.
 * @param &remaining The remaining of the select clause.
 * @return Vector of return synonyms (empty if BOOLEAN).
 */
vector<string> QueryTokenizer::tokenizeReturnSynonyms(string input,
                                                      string &remaining) {
    size_t nextWhitespacePos = findNextWhitespace(input, 0);
    string keyword = input.substr(0, nextWhitespacePos);
    if (nextWhitespacePos == string::npos || keyword != KEYWORD_SELECT) {
        throw SyntaxError("QP-ERROR: invalid select clause");
    }

    string rest = trimL(input.substr(nextWhitespacePos + 1));

    // check if <
    size_t lCarrotPos = rest.find(L_CARROT);
    if (lCarrotPos != string::npos) {
        rest = trimL(rest.substr(lCarrotPos + 1));
        vector<string> retStrings = tokenizeReturnTuple(rest, remaining);
        return retStrings;
    }

    string retString = tokenizeReturnRef(rest, remaining);
    if (retString == KEYWORD_BOOLEAN) {
        return vector<string>();
    }
    return vector<string>{retString};
}

vector<string> QueryTokenizer::tokenizeReturnTuple(string input,
                                                   string &remaining) {
    size_t rCarrotPos = input.find(R_CARROT);
    if (rCarrotPos == string::npos) {
        throw SyntaxError("QP-ERROR: invalid return tuple");
    }

    string tuple = trimR(input.substr(0, rCarrotPos));

    vector<string> retStrings = tokenizeCommaSeparatedValues(tuple);
    for (size_t i = 0; i < retStrings.size(); ++i) {
        retStrings[i] = removeWhitespaceAroundPeriod(retStrings[i]);
    }
    remaining = trimL(input.substr(rCarrotPos + 1));
    return retStrings;
}

string QueryTokenizer::tokenizeReturnRef(string input, string &remaining) {
    size_t nextWhitespacePos = findNextWhitespace(input, 0);

    string synonym = input.substr(0, nextWhitespacePos);

    if (nextWhitespacePos == string::npos) {
        remaining = EMPTY_STR;
        return synonym;
    }

    size_t periodPos = input.find(PERIOD);

    if (periodPos != string::npos) {
        size_t tokenPos = findNextToken(input, nextWhitespacePos);
        if (periodPos == tokenPos) {
            // x .( )y
            tokenPos = findNextToken(input, tokenPos + 1);
            nextWhitespacePos = findNextWhitespace(input, tokenPos);
        } else if (periodPos < tokenPos && periodPos == nextWhitespacePos - 1) {
            // x. y
            nextWhitespacePos = findNextWhitespace(input, tokenPos);
        }
    }

    remaining = trimL(input.substr(nextWhitespacePos));
    string returnRef = input.substr(0, nextWhitespacePos);
    return removeWhitespaceAroundPeriod(returnRef);
}

/**
 * Tokenizes each clause into individual clause tuples and adds them to a vector
 * based on what kind of clause, i.e. such that, pattern or with.
 * @param input The clauses string (assumed non-empty).
 * @param &suchThatClauses Vector of such that tuples.
 * @param &patternClauses Vector of pattern tuples.
 * @param &withClauses Vector of with tuples.
 */
void QueryTokenizer::tokenizeClauses(string input,
                                     vector<ClsTuple> &suchThatClauses,
                                     vector<PatTuple> &patternClauses,
                                     vector<WithPair> &withClauses) {
    // State enums will be declared within class
    // unless a more elegant method is found
    enum { SUCH_THAT_CLAUSE, PATTERN_CLAUSE, WITH_CLAUSE, NULL_CLAUSE };

    enum {
        READ_TYPE_STATE,
        SUCH_INTERMEDIATE_STATE,
        READ_ARGS_STATE,
        INVALID_STATE
    };

    size_t tokenPos = findNextToken(input, 0);
    size_t state = READ_TYPE_STATE;
    size_t type = NULL_CLAUSE;
    while (tokenPos != string::npos) {
        size_t whitespacePos = findNextWhitespace(input, tokenPos);
        string token = input.substr(tokenPos, whitespacePos - tokenPos);

        switch (state) {
        case READ_TYPE_STATE:
            if (token == KEYWORD_SUCH) {
                state = SUCH_INTERMEDIATE_STATE;
            } else if (token == KEYWORD_PATTERN) {
                state = READ_ARGS_STATE;
                type = PATTERN_CLAUSE;
            } else if (token == KEYWORD_WITH) {
                state = READ_ARGS_STATE;
                type = WITH_CLAUSE;
            } else if (token == KEYWORD_AND) {
                state = READ_ARGS_STATE;
            } else {
                state = INVALID_STATE;
            }
            break;
        case SUCH_INTERMEDIATE_STATE:
            if (token == KEYWORD_THAT) {
                state = READ_ARGS_STATE;
                type = SUCH_THAT_CLAUSE;
            } else {
                state = INVALID_STATE;
            }
            break;
        case READ_ARGS_STATE:
            state = READ_TYPE_STATE;
            switch (type) {
            case SUCH_THAT_CLAUSE: {
                ClsTuple clause;
                whitespacePos = tokenizeSuchThatClause(input, tokenPos, clause);
                suchThatClauses.push_back(clause);
                break;
            }
            case PATTERN_CLAUSE: {
                PatTuple clause;
                whitespacePos = tokenizePatternClause(input, tokenPos, clause);
                patternClauses.push_back(clause);
                break;
            }
            case WITH_CLAUSE: {
                WithPair clause;
                whitespacePos = tokenizeWithClause(input, tokenPos, clause);
                withClauses.push_back(clause);
                break;
            }
            }
            break;
        }
        if (state == INVALID_STATE) {
            throw SyntaxError("QP-ERROR: error in the matrix.");
        }

        // find the start position of the next token
        tokenPos = findNextToken(input, whitespacePos);
    }
    if (state != READ_TYPE_STATE) {
        throw SyntaxError("QP-ERROR: invalid clause syntax");
    }
    return;
}

/**
 * Tokenizes a such that clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause ClsTuple.
 * @return Position of end of clause.
 */
size_t QueryTokenizer::tokenizeSuchThatClause(string input, size_t startPos,
                                              ClsTuple &clause) {
    string token1;
    string token2;
    string token3;
    size_t nextPos;

    // tokenize token before '(', ',', ')'
    token1 = getTokenBeforeX(input, L_BRACKET, startPos, nextPos);
    validateRsType(token1);

    token2 = getTokenBeforeX(input, COMMA, nextPos, nextPos);
    token3 = getTokenBeforeX(input, R_BRACKET, nextPos, nextPos);

    // remove whitespace within quotes of token2 and token3 if any!
    token2 = removeWhitespaceWithinQuotes(token2);
    token3 = removeWhitespaceWithinQuotes(token3);

    validateClauseArg(token2);
    validateClauseArg(token3);
    clause = make_tuple(token1, token2, token3);
    return nextPos;
}

/**
 * Tokenizes a pattern clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause PatTuple.
 * @return Position of end of clause.
 */
size_t QueryTokenizer::tokenizePatternClause(string input, size_t startPos,
                                             PatTuple &clause) {
    PatIdent ident;
    PatVar var;
    vector<PatToken> tokens;
    vector<string> arguments;
    size_t nextPos;
    size_t commaPos;
    string token;

    ident = getTokenBeforeX(input, L_BRACKET, startPos, nextPos);

    commaPos = input.find(COMMA, nextPos);
    if (commaPos == string::npos) {
        throw SyntaxError("QP-ERROR: no comma after left bracket");
    }

    // go through the commas
    while (commaPos != string::npos) {
        string token = trim(input.substr(nextPos, commaPos - nextPos));
        if (ParserUtil::isQuoted(token) || hasNoWhitespace(token) ||
            ParserUtil::isWildcard(token)) {
            token = removeWhitespaceWithinQuotes(token);
            arguments.push_back(token);
            nextPos = commaPos + 1;
        } else {
            // walked into another clause's comma
            break;
        }
        commaPos = input.find(COMMA, nextPos);
    }

    size_t tokenPos = findNextToken(input, nextPos);
    if (tokenPos == string::npos) {
        throw SyntaxError("QP-ERROR: incomplete pattern clause");
    }

    if (input[tokenPos] == QUOTE) {
        tokenPos = tokenPos + 1;
        token = getTokenBeforeX(input, QUOTE, tokenPos, nextPos);
        token = QUOTE_STR + token + QUOTE_STR;
        // check for R_BRACKET
        nextPos = getPosAfterRBracket(input, nextPos);
    } else if (input[tokenPos] == UNDERSCORE) {
        tokenPos = tokenPos + 1;
        tokenPos = findNextToken(input, tokenPos);
        if (input[tokenPos] == R_BRACKET) {
            nextPos = tokenPos + 1;
            token = UNDERSCORE_STR;
        } else {
            token = getTokenBeforeX(input, UNDERSCORE, tokenPos, nextPos);
            if (!ParserUtil::isQuoted(token)) {
                throw SyntaxError(
                    "QP-ERROR: pattern underscore not followed by quotes");
            }
            token = UNDERSCORE_STR + removeWhitespaceWithinQuotes(token) +
                    UNDERSCORE_STR;
            // check for R_BRACKET
            nextPos = getPosAfterRBracket(input, nextPos);
        }
    } else {
        token = getTokenBeforeX(input, R_BRACKET, tokenPos, nextPos);
    }
    arguments.push_back(token);

    var = arguments.at(0);
    validateClauseArg(var);

    for (size_t i = 1; i < arguments.size(); ++i) {
        tokens.push_back(arguments.at(i));
    }

    clause = make_tuple(ident, var, tokens);
    return nextPos;
}

/**
 * Tokenizes a with clause into a pair.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause WithPair.
 * @return Position of end of clause.
 */
size_t QueryTokenizer::tokenizeWithClause(string input, size_t startPos,
                                          WithPair &clause) {

    string token1;
    string token2;
    size_t nextPos;
    size_t tempPos;

    // '"' IDENT '"' | INTEGER | attrRef | synonym

    token1 = getTokenBeforeX(input, EQUAL, startPos, nextPos);
    token1 = removeWhitespaceWithinQuotes(token1);
    token1 = removeWhitespaceAroundPeriod(token1);

    tempPos = findNextToken(input, nextPos);
    if (tempPos == string::npos) {
        throw SyntaxError("QP-ERROR: incomplete with clause");
    }
    if (input[tempPos] == QUOTE) {
        tempPos = tempPos + 1;
        token2 = getTokenBeforeX(input, QUOTE, tempPos, nextPos);
        token2 = QUOTE_STR + token2 + QUOTE_STR;
    } else {
        size_t periodPos = input.find(PERIOD, tempPos);
        nextPos = findNextWhitespace(input, tempPos);
        if (periodPos != string::npos) {
            size_t tokenPos = findNextToken(input, nextPos);
            if (periodPos == tokenPos) {
                tokenPos = findNextToken(input, periodPos + 1);
                nextPos = findNextWhitespace(input, tokenPos);
            } else if (periodPos == nextPos - 1) {
                nextPos = findNextWhitespace(input, tokenPos);
            }
        }
        token2 = input.substr(tempPos, nextPos - tempPos);
        token2 = removeWhitespaceAroundPeriod(token2);
    }

    validateClauseArg(token1);
    validateClauseArg(token2);
    clause = make_pair(token1, token2);
    return nextPos;
}

vector<PatToken> QueryTokenizer::tokenizePattern(vector<string> patArgs) {
    if (isPatternArgumentWildcard(patArgs)) {
        return patArgs;
    }

    if (patArgs.size() != PATARG_SIZE_ONE) {
        throw SyntaxError("QP-ERROR: assign should have one pattern argument");
    }

    string pattern = patArgs.at(0);

    vector<PatToken> tokens;
    string token;
    size_t tokenPos;
    size_t delimiterPos;

    tokenPos = findNextToken(pattern, 0);
    while (tokenPos != string::npos) {
        char c = pattern[tokenPos];
        delimiterPos = findPatternDelimiter(pattern, tokenPos);
        if (delimiterPos == string::npos) {
            token = trimR(pattern.substr(tokenPos));
            tokens.push_back(token);
            break;
        }
        if (delimiterPos == tokenPos) {
            token = string(1, c);
            tokenPos = findNextToken(pattern, delimiterPos + 1);
        } else {
            token = trimR(pattern.substr(tokenPos, delimiterPos - tokenPos));
            tokenPos = findNextToken(pattern, delimiterPos);
        }
        tokens.push_back(token);
    }

    validateTokens(tokens);
    return tokens;
}

void QueryTokenizer::validateTokens(vector<PatToken> tokens) {
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
            } else if (ParserUtil::isValidName(t) || ParserUtil::isInteger(t)) {
                isWord = false;
            } else {
                throw SyntaxError("QP-ERROR: invalid pattern string");
            }
        } else {
            if (isOperator(t)) {
                isWord = true;
            } else if (isRBracket(t)) {
                isWord = false;
                bracketCount -= 1;
            } else {
                throw SyntaxError("QP-ERROR: invalid pattern string");
            }
        }
    }

    if (bracketCount != 0 || isWord == true) {
        throw SyntaxError("QP-ERROR: invalid pattern string");
    }
}

/****************** tokenization helper functions ******************/

/**
 * Tokenizes the comma separated values.
 * Used by both tuple tokenization and synonym declaration.
 * @param input Comma separated values.
 * @return Vector of synonym strings.
 */
vector<string> QueryTokenizer::tokenizeCommaSeparatedValues(string input) {
    vector<string> syns;
    string remaining = input;

    size_t nextCommaPos = remaining.find(COMMA);
    while (nextCommaPos != string::npos) {
        string s = trimR(remaining.substr(0, nextCommaPos));
        syns.push_back(s);

        remaining = trimL(remaining.substr(nextCommaPos + 1));
        nextCommaPos = remaining.find(COMMA);
    }

    // last synonym with no comma behind
    if (remaining.empty()) {
        throw SyntaxError("QP-ERROR: CSV is empty or ends on comma");
    }
    syns.push_back(remaining);
    return syns;
}

/**
 * Given a quoted string, removes the whitespace within the quotes.
 * E.g. "    someprocedure     " => "someprocedure".
 * E.g. "    some procedure    " => "some procedure".
 * E.g. someprocedure            => someprocedure.
 * @param input The quoted string.
 * @return The quoted string with whitespace removed.
 */
string QueryTokenizer::removeWhitespaceWithinQuotes(string input) {
    if (count(input.begin(), input.end(), QUOTE) > 0) {
        if (input[0] == QUOTE && input[input.size() - 1] == QUOTE) {
            string trimmed = trimQuotes(input);
            return QUOTE_STR + trimmed + QUOTE_STR;
        } else {
            throw SyntaxError("QP-ERROR: misplaced quotes");
        }
    }
    return input;
}

/**
 * Given a string with period, removes whitespace around the period.
 * E.g. x.y   => x.y.
 * E.g. x . y => x.y.
 * E.g. x     => x.
 * @param input The quoted string.
 * @return The quoted string with whitespace removed.
 */
string QueryTokenizer::removeWhitespaceAroundPeriod(string input) {
    size_t periodPos = input.find(PERIOD);
    if (periodPos != string::npos) {
        string token1 = trim(input.substr(0, periodPos));
        string token2 = trim(input.substr(periodPos + 1));
        return token1 + PERIOD_STR + token2;
    }
    return input;
}

/****************** validation functions ******************/

/**
 * Validates input to be valid design entity type.
 * @param input The input string.
 * @exception SyntaxError if design entity type is invalid.
 */
void QueryTokenizer::validateDeType(string input) {
    if (DE_TYPE_SET.find(input) == DE_TYPE_SET.end()) {
        throw SyntaxError("QP-ERROR: invalid design entity type");
    }
}

/**
 * Validates input to be valid relationship type.
 * @param input The input string.
 * @exception SyntaxError if relationship type is invalid.
 */
void QueryTokenizer::validateRsType(string input) {
    if (RS_TYPE_SET.find(input) == RS_TYPE_SET.end()) {
        throw SyntaxError("QP-ERROR: invalid relationship type");
    }
}

/**
 * Validates input to be valid clause argument.
 * @param input The input string.
 * @exception SyntaxError if clause argument is invalid.
 */
void QueryTokenizer::validateClauseArg(string input) {
    if (ParserUtil::isQuoted(input)) {
        validateQuoted(input);
    } else if (ParserUtil::isAttrRef(input)) {
        validateAttrRef(input);
    }
}

/**
 * Validates input to be valid quoted string.
 * @param input The input string.
 * @exception SyntaxError if quoted string is invalid.
 */
void QueryTokenizer::validateQuoted(string input) {
    string trimmed = trimQuotes(input);
    validateName(trimmed);
}

/**
 * Validates input to be valid attribute reference.
 * @param input The input string.
 * @exception SyntaxError if attribute reference is invalid.
 */
void QueryTokenizer::validateAttrRef(string input) {
    AttrRef ref = ParserUtil::splitAttrRef(input);
    validateName(ref.first);
    validateAttribute(ref.second);
}

/**
 * Validates input to be valid name.
 * @param input The input string.
 * @exception SyntaxError if name is invalid.
 */
void QueryTokenizer::validateName(string input) {
    if (!ParserUtil::isValidName(input)) {
        throw SyntaxError("QP-ERROR: invalid name");
    }
}

/**
 * Validates input to be valid attribute.
 * @param input The input string.
 * @exception SyntaxError if attribute is invalid.
 */
void QueryTokenizer::validateAttribute(string input) {
    if (ATTRIBUTE_SET.find(input) == ATTRIBUTE_SET.end()) {
        throw SyntaxError("QP-ERROR: invalid attribute reference");
    }
}

/****************** lower level helper functions *******************/

/**
 * Removes whitespace from the front and back of a given string.
 * @param input The input string.
 * @return The trimmed string, or empty string if there is no string to trim.
 */
string QueryTokenizer::trim(string input) {
    size_t firstPos = input.find_first_not_of(WHITESPACE_SET);
    size_t lastPos = input.find_last_not_of(WHITESPACE_SET);
    if (firstPos != string::npos && lastPos != string::npos) {
        return input.substr(firstPos, lastPos - firstPos + 1);
    }
    return EMPTY_STR;
}

/**
 * Removes whitespace from the front of a given string.
 * @param input The input string.
 * @return The trimmed string, or empty string if there is no string to trim.
 */
string QueryTokenizer::trimL(string input) {
    size_t firstPos = input.find_first_not_of(WHITESPACE_SET);
    if (firstPos != string::npos) {
        return input.substr(firstPos);
    }
    return EMPTY_STR;
}

/**
 * Removes whitespace from the back of a given string.
 * @param input The input string.
 * @return The trimmed string, or empty string if there is no string to trim.
 */
string QueryTokenizer::trimR(string input) {
    size_t lastPos = input.find_last_not_of(WHITESPACE_SET);
    if (lastPos != string::npos) {
        return input.substr(0, lastPos + 1);
    }
    return EMPTY_STR;
}

/**
 * Removes quotes and whitespace from the front and back of a given string.
 * @param input The input string.
 * @return The trimmed string, or empty string if there is no string to trim.
 */
string QueryTokenizer::trimQuotes(string input) {
    return trim(input.substr(1, input.size() - 2));
}

size_t QueryTokenizer::findNextWhitespace(string input, size_t pos) {
    return input.find_first_of(WHITESPACE_SET, pos);
}

size_t QueryTokenizer::findNextToken(string input, size_t pos) {
    return input.find_first_not_of(WHITESPACE_SET, pos);
}

size_t QueryTokenizer::findPatternDelimiter(string input, size_t pos) {
    return input.find_first_of(PATTERN_DELIMITER_SET, pos);
}

/**
 * Get token starting from a start positon and before a character x.
 * @param input The clauses string.
 * @param startPos The start positon.
 * @param x The character to find.
 * @param &nextPos Position after X.
 * @return The token to retrieve.
 */
string QueryTokenizer::getTokenBeforeX(string input, char x, size_t startPos,
                                       size_t &nextPos) {
    size_t xPos = input.find(x, startPos);
    if (xPos == string::npos) {
        throw SyntaxError("QP-ERROR: missing " + x);
    }

    nextPos = xPos + 1;
    return trim(input.substr(startPos, xPos - startPos));
}

size_t QueryTokenizer::getPosAfterRBracket(string input, size_t startPos) {
    size_t tokenPos = findNextToken(input, startPos);
    if (input[tokenPos] != R_BRACKET) {
        throw SyntaxError("QP-ERROR: missing right bracket");
    }

    return tokenPos + 1;
}

bool QueryTokenizer::hasNoWhitespace(string input) {
    return findNextWhitespace(input, 0) == string::npos;
}

/**
 * Evaluates whether pattern arguments are wildcards.
 * @param patArgs Pattern arguments
 * @return true if all wildcards and correct size, otherwise false.
 */
bool QueryTokenizer::isPatternArgumentWildcard(vector<string> patArgs) {
    return all_of(patArgs.begin(), patArgs.end(),
                  [](string x) { return x == UNDERSCORE_STR; }) &&
           (patArgs.size() == PATARG_SIZE_ONE ||
            patArgs.size() == PATARG_SIZE_TWO);
}

bool QueryTokenizer::isOperator(string token) {
    return OPERATOR_SET.find(token) != string::npos;
}

bool QueryTokenizer::isLBracket(string token) { return token == "("; }

bool QueryTokenizer::isRBracket(string token) { return token == ")"; }
