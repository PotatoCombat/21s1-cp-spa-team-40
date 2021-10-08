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
        throw SyntaxError("QP-ERROR: no declarations");
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

        deHelper.valueToDesType(type); // throws exception if not a type

        // synoyms lie between whitespace and semicolon
        string synonyms = trim(input.substr(
            firstWhitespacePos, nextSemicolonPos - firstWhitespacePos));

        // add tuples to the vector
        vector<string> syns = tokenizeCommaSeparatedValues(synonyms);
        for (auto s : syns) {
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
        throw SyntaxError("QP-ERROR: no select clause");
    }

    string rest = trimL(input.substr(nextWhitespacePos + 1));

    vector<string> retStrings;

    // check if <
    size_t lCarrotPos = rest.find(L_CARROT);
    if (lCarrotPos != string::npos) {
        rest = trimL(rest.substr(lCarrotPos + 1));
        retStrings = tokenizeReturnTuple(rest, remaining);
    } else {
        string retString = tokenizeReturnRef(rest, remaining);
        if (retString != KEYWORD_BOOLEAN) {
            retStrings.push_back(retString);
        }
    }

    for (int i = 0; i < retStrings.size(); ++i) {
        retStrings[i] = removeWhitespaceAroundPeriod(retStrings[i]);
    }
    return retStrings;

    /*size_t nextWhitespacePos = findNextWhitespace(rest, 0);

    string retString = rest.substr(0, nextWhitespacePos);
    if (nextWhitespacePos == string::npos) {
        remaining = "";
    } else {
        size_t periodPos = rest.find(PERIOD, 0);
        size_t nextTokenPos = findNextToken(rest, nextWhitespacePos);
        if (periodPos != string::npos) {
            if (periodPos == nextTokenPos) {
                rest = trimL(rest.substr(nextTokenPos + 1));
                nextWhitespacePos = findNextWhitespace(rest, 0);
                retString = retString + "." + rest.substr(0, nextWhitespacePos);
            } else if (periodPos < nextTokenPos &&
                       periodPos == nextWhitespacePos - 1) {
                nextWhitespacePos = findNextWhitespace(rest, nextTokenPos);
                retString =
                    retString +
                    rest.substr(nextTokenPos, nextWhitespacePos - nextTokenPos);
            }
        }
        remaining = trimL(rest.substr(nextWhitespacePos + 1));
    }

    if (retString == KEYWORD_BOOLEAN) {
        return vector<string>();
    }
    return vector<string>{retString};*/
}

vector<string> QueryTokenizer::tokenizeReturnTuple(string input,
                                                   string &remaining) {
    size_t rCarrotPos = input.find(R_CARROT);
    if (rCarrotPos == string::npos) {
        throw SyntaxError("QP-ERROR: invalid return tuple");
    }

    string tuple = trimR(input.substr(0, rCarrotPos));

    vector<string> retStrings = tokenizeCommaSeparatedValues(tuple);
    for (int i = 0; i < retStrings.size(); ++i) {
        retStrings[i] = removeWhitespaceAroundPeriod(retStrings[i]);
    }
    remaining = trimL(input.substr(rCarrotPos + 1));
    return retStrings;
}

string QueryTokenizer::tokenizeReturnRef(string input, string &remaining) {
    size_t nextWhitespacePos = findNextWhitespace(input, 0);

    string synonym = input.substr(0, nextWhitespacePos);

    if (nextWhitespacePos == string::npos) {
        remaining = "";
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
    return input.substr(0, nextWhitespacePos);
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
                                     vector<WithTuple> &withClauses) {
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
                whitespacePos = tokenizeSuchThat(input, tokenPos, clause);
                suchThatClauses.push_back(clause);
                break;
            }
            case PATTERN_CLAUSE: {
                PatTuple clause;
                whitespacePos = tokenizePattern(input, tokenPos, clause);
                patternClauses.push_back(clause);
                break;
            }
            case WITH_CLAUSE: {
                WithTuple clause;
                whitespacePos = tokenizeWith(input, tokenPos, clause);
                withClauses.push_back(clause);
                break;
            }
            default:
                // won't reach here
                throw SyntaxError("QP-ERROR: error in the matrix.");
            }
            break;
        default: // INVALID_STATE
            // won't reach here
            throw SyntaxError("QP-ERROR: error in the matrix.");
        }
        if (state == INVALID_STATE) {
            throw SyntaxError("QP-ERROR: error in the matrix.");
        }

        // find the start position of the next token
        tokenPos = findNextToken(input, whitespacePos);
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
size_t QueryTokenizer::tokenizeSuchThat(string input, size_t startPos,
                                        ClsTuple &clause) {
    string token1;
    string token2;
    string token3;
    size_t nextPos;

    // tokenize token before '(', ',', ')'
    token1 = getTokenBeforeX(input, L_BRACKET, startPos, nextPos);
    token2 = getTokenBeforeX(input, COMMA, nextPos, nextPos);
    token3 = getTokenBeforeX(input, R_BRACKET, nextPos, nextPos);

    // remove whitespace within quotes of token2 and token3 if any!
    token2 = removeWhitespaceWithinQuotes(token2);
    token3 = removeWhitespaceWithinQuotes(token3);

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
size_t QueryTokenizer::tokenizePattern(string input, size_t startPos,
                                       PatTuple &clause) {
    string ident;
    vector<PatArg> arguments;
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
        if (isQuotedString(token) || hasNoWhitespace(token) ||
            isWildcard(token)) {
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
    if (input[tokenPos] == QUOTE) {
        tokenPos = tokenPos + 1;
        token = getTokenBeforeX(input, QUOTE, tokenPos, nextPos);
        token = "\"" + token + "\"";
        // check for R_BRACKET
        nextPos = getPosAfterRBracket(input, nextPos);
    } else if (input[tokenPos] == UNDERSCORE) {
        tokenPos = tokenPos + 1;
        tokenPos = findNextToken(input, tokenPos);
        if (input[tokenPos] == R_BRACKET) {
            nextPos = tokenPos + 1;
            token = "_";
        } else {
            token =
                getTokenBeforeX(input, UNDERSCORE, tokenPos, nextPos);
            token = "_" + removeWhitespaceWithinQuotes(token) + "_";
            // check for R_BRACKET
            nextPos = getPosAfterRBracket(input, nextPos);
        }
    } else {
        token = getTokenBeforeX(input, R_BRACKET, tokenPos, nextPos);
    }

    arguments.push_back(token);
    clause = make_pair(ident, arguments);
    return nextPos;
}

/**
 * Tokenizes a with clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause WithTuple.
 * @return Position of end of clause.
 * @todo Fix algo and clean up
 */
size_t QueryTokenizer::tokenizeWith(string input, size_t startPos,
                                    WithTuple &clause) {
    string temp;
    string token1 = "";
    string token2 = "";
    string token3 = "";
    string token4 = "";
    //size_t nextPos;
    size_t nextPos = startPos;
    size_t tempPos;

    // tokenize token by '=' and '.'
    // structure: token1.token2 = token3.token4
    // structure: "    name   " = token3.token4 // name stored in token1
    // structure: token1.token2 = 24 // 24 stored in token 3
    //temp = getTokenBeforeX(input, EQUAL, startPos, nextPos);
    //size_t periodPos = temp.find(PERIOD);
    //if (periodPos == string::npos) {
    //    token1 = temp;
    //    token2 = "";
    //} else {
    //    token1 = trim(temp.substr(0, periodPos));
    //    token2 = trim(temp.substr(periodPos + 1));
    //}

    //periodPos = input.find(PERIOD, nextPos);

    //if (periodPos == string::npos) { // no '.'
    //    // TODO: Handle "     name      "
    //    tempPos = findNextWhitespace(input, nextPos);
    //    temp = input.substr(nextPos, tempPos - nextPos);
    //    token3 = trim(temp);
    //    token4 = "";
    //    nextPos = tempPos;
    //} else {
    //    size_t startTokenPos = findNextToken(input, nextPos);
    //    size_t endTokenPos = findNextWhitespace(input, startTokenPos);
    //    string tempToken =
    //        input.substr(startTokenPos, endTokenPos - startTokenPos);
    //    temp = trim(input.substr(nextPos, nextPos - periodPos));
    //    if (temp == tempToken) {
    //        token3 = temp;
    //        startTokenPos = findNextToken(input, periodPos + 1);
    //        endTokenPos = findNextWhitespace(input, startTokenPos);
    //        token4 = input.substr(startTokenPos, endTokenPos - startTokenPos);
    //    } else { // no '.'
    //        // TODO: Handle "     name      "
    //        token3 = tempToken;
    //        token4 = "";
    //    }
    //    nextPos = endTokenPos;
    //}

    //// remove whitespace within quotes of token1 and token3 if any!
    //token1 = removeWhitespaceWithinQuotes(token1);
    //token3 = removeWhitespaceWithinQuotes(token3);

    clause = vector<WithArg>{token1, token2, token3, token4};
    return nextPos;
}

/********************* helper functions *********************/

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
    return "";
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
    return "";
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
    return "";
}

size_t QueryTokenizer::findNextWhitespace(string input, size_t pos) {
    return input.find_first_of(WHITESPACE_SET, pos);
}

size_t QueryTokenizer::findNextToken(string input, size_t pos) {
    return input.find_first_not_of(WHITESPACE_SET, pos);
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
    if (input[tokenPos] == R_BRACKET) {
        return tokenPos + 1;
    }
    throw SyntaxError("QP-ERROR: missing right bracket");
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
            string trimmed = trim(input.substr(1, input.size() - 2));
            return "\"" + trimmed + "\"";
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
    if (periodPos == string::npos) {
        return input;
    }
    string token1 = trim(input.substr(0, periodPos));
    string token2 = trim(input.substr(periodPos + 1));
    return token1 + "." + token2;
}

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
    if (!remaining.empty()) {
        syns.push_back(remaining);
    }
    return syns;
}

string QueryTokenizer::extractPatternString(string input) {
    if (count(input.begin(), input.end(), QUOTE) == 2) {
        size_t n_underscore = count(input.begin(), input.end(), UNDERSCORE);
        if (n_underscore == 2) {
            string underscored_r = trim(input.substr(1, input.size() - 2));
            return trim(underscored_r.substr(1, underscored_r.size() - 2));
        } else if (n_underscore == 1) {
            throw SyntaxError("QP-ERROR: invalid pattern syntax");
        }
        return trim(input.substr(1, input.size() - 2));
    }
    return input;
}

bool QueryTokenizer::isWildcard(string input) { return input == "_"; }

bool QueryTokenizer::isQuotedString(string input) {
    if (count(input.begin(), input.end(), QUOTE) == 2 && input[0] == QUOTE &&
        input[input.size() - 1] == QUOTE) {
        return true;
    }
    return false;
}

bool QueryTokenizer::hasNoWhitespace(string input) {
    size_t pos = findNextWhitespace(input, 0);
    return pos == string::npos;
}
