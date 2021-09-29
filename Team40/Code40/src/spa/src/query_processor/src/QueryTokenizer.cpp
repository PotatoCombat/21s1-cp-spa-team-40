#include "query_processor/QueryTokenizer.h"

pair<string, string> QueryTokenizer::separateQueryString(string input) {
    size_t lastSemicolon = input.rfind(SEMICOLON);

    if (lastSemicolon == string::npos) {
        throw SyntaxError("QP-ERROR: no declarations");
    }

    string decl = trim(input.substr(0, lastSemicolon + 1));
    string slct = trim(input.substr(lastSemicolon + 1));
    return make_pair(decl, slct);
}

void QueryTokenizer::tokenizeDeclarations(string input, vector<DeclPair> &decls) {
    size_t nextSemicolonPos = input.find(SEMICOLON);
    while (nextSemicolonPos != string::npos) {
        size_t firstWhitespacePos = findNextWhitespace(input, 0);
        string type = input.substr(0, firstWhitespacePos);

        deHelper.getType(type); // throws exception if not a type

        // synoyms lie between whitespace and semicolon
        string synonyms = trim(input.substr(
            firstWhitespacePos, nextSemicolonPos - firstWhitespacePos));

        // add tuples to the vector
        vector<string> syns = tokenizeDeclarationSynonym(synonyms);
        for (auto s : syns) {
            decls.push_back(make_pair(type, s));
        }

        // prepare for next iteration of while loop
        input = trimL(input.substr(nextSemicolonPos + 1));
        nextSemicolonPos = input.find(SEMICOLON);
    }
    return;
}

vector<string> QueryTokenizer::tokenizeDeclarationSynonym(string input) {
    vector<string> syns;
    string remaining = input;

    size_t nextCommaPos = remaining.find(COMMA);
    while (nextCommaPos != string::npos) {
        string s = trimR(remaining.substr(0, nextCommaPos));
        s = parseValidName(s);
        syns.push_back(s);

        remaining = trimL(remaining.substr(nextCommaPos + 1));
        nextCommaPos = remaining.find(COMMA);
    }

    // last synonym with no comma behind
    if (!remaining.empty()) {
        string s = parseValidName(remaining);
        syns.push_back(s);
    }
    return syns;
}

void QueryTokenizer::tokenizeReturnSynonym(string input, string &returnSynonym,
                           string &remaining) {
    size_t nextWhitespacePos = findNextWhitespace(input, 0);
    string keyword = input.substr(0, nextWhitespacePos);
    if (nextWhitespacePos == string::npos || keyword != KEYWORD_SELECT) {
        throw SyntaxError("QP-ERROR: no select clause");
    }

    string rest = trimL(input.substr(nextWhitespacePos + 1));

    nextWhitespacePos = findNextWhitespace(rest, 0);

    if (nextWhitespacePos == string::npos) {
        returnSynonym = rest;
        remaining = "";
        return;
    }

    returnSynonym = rest.substr(0, nextWhitespacePos);
    remaining = trimL(rest.substr(nextWhitespacePos + 1));
    return;

    // size_t nextWhitespacePos = findNextWhitespace(input, 0);
    // string keyword = input.substr(0, nextWhitespacePos);
    // if (nextWhitespacePos == string::npos || keyword != KEYWORD_SELECT) {
    //    throw SyntaxError("QP-ERROR: no select clause");
    // }
    //
    // size_t nextTokenPos = findNextToken(input, nextWhitespacePos + 1);
    // nextWhitespacePos = findNextWhitespace(input, nextTokenPos);
    // if (nextWhitespacePos == string::npos) {
    //    remaining = "";
    // } else {
    //    remaining = trimL(input.substr(nextWhitespacePos + 1));
    // }
    // returnSynonym = input.substr(nextTokenPos, nextWhitespacePos -
    // nextTokenPos); return;
}

void QueryTokenizer::tokenizeClauses(string input, vector<ClsTuple> &suchThatClauses,
                     vector<PatTuple> &patternClauses,
                     vector<WithTuple> &withClauses) {
    size_t tokenPos = findNextToken(input, 0);
    enum STATE_CODES state = READ_TYPE_STATE;
    enum CLAUSE_CODES type = NULL_CLAUSE;
    while (tokenPos != string::npos) {
        size_t whitespacePos = findNextWhitespace(input, tokenPos);
        string token = input.substr(tokenPos, whitespacePos - tokenPos);

        switch (state) {
        case READ_TYPE_STATE: {
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
        }
        case SUCH_INTERMEDIATE_STATE: {
            if (token == KEYWORD_THAT) {
                state = READ_ARGS_STATE;
                type = SUCH_THAT_CLAUSE;
            } else {
                state = INVALID_STATE;
            }
            break;
        }
        case READ_ARGS_STATE: {
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
                break;
            }
            break;
        }
        default: // INVALID_STATE
            // won't reach here
            throw SyntaxError("QP-ERROR: error in the matrix.");
            break;
        }
        if (state == INVALID_STATE) {
            throw SyntaxError("QP-ERROR: error in the matrix.");
        }

        // find the start position of the next token
        tokenPos = findNextToken(input, whitespacePos);
    }
    return;
}

size_t QueryTokenizer::tokenizeSuchThat(string input, size_t startPos, ClsTuple &clause) {
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

size_t QueryTokenizer::tokenizePattern(string input, size_t startPos, PatTuple &clause) {
    string token1;
    string token2;
    string token3;
    size_t nextPos;

    // ASSUMES '(', ',', ')'
    token1 = getTokenBeforeX(input, L_BRACKET, startPos, nextPos);
    token2 = getTokenBeforeX(input, COMMA, nextPos, nextPos);
    token3 = getTokenBeforeX(input, R_BRACKET, nextPos, nextPos);

    // remove whitespace within quotes of token2 and token3 if any!
    token2 = removeWhitespaceWithinQuotes(token2);
    token3 = extractPatternString(token3);

    clause = make_tuple(token1, token2, token3);
    return nextPos;
}

size_t QueryTokenizer::tokenizeWith(string input, size_t startPos, WithTuple &clause) {
    string temp;
    string token1;
    string token2;
    string token3;
    string token4;
    size_t nextPos;

    // tokenize token by '=' and '.'
    // structure: token1.token2 = token3.token4
    // structure: "    name   " = token3.token4 // name stored in token1
    // structure: token1.token2 = 24 // 24 stored in token 3
    temp = getTokenBeforeX(input, EQUAL, startPos, nextPos);
    size_t periodPos = temp.find(PERIOD);
    if (periodPos == string::npos) {
        token1 = temp;
        token2 = "";
    } else {
        token1 = temp.substr(0, periodPos);
        token2 = temp.substr(periodPos + 1);
    }

    temp = getTokenBeforeX(input, EQUAL, nextPos, nextPos);
    size_t periodPos = temp.find(PERIOD);
    if (periodPos == string::npos) {
        token3 = temp;
        token4 = "";
    } else {
        token3 = temp.substr(0, periodPos);
        token4 = temp.substr(periodPos + 1);
    }

    // remove whitespace within quotes of token1 and token3 if any!
    token1 = removeWhitespaceWithinQuotes(token1);
    token3 = removeWhitespaceWithinQuotes(token3);

    clause = vector<WithArg>{token1, token2, token3, token4};
    return nextPos;
}

/********************* helper functions *********************/

string QueryTokenizer::trim(string input) {
    size_t firstPos = input.find_first_not_of(WHITESPACE_SET);
    size_t lastPos = input.find_last_not_of(WHITESPACE_SET);
    if (firstPos != string::npos && lastPos != string::npos) {
        return input.substr(firstPos, lastPos - firstPos + 1);
    }
    return "";
}

string QueryTokenizer::trimL(string input) {
    size_t firstPos = input.find_first_not_of(WHITESPACE_SET);
    if (firstPos != string::npos) {
        return input.substr(firstPos);
    }
    return "";
}

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

string QueryTokenizer::parseValidName(string input) {
    if (isalpha(input[0])) {
        auto it = find_if_not(begin(input), end(input), isalnum);
        if (it == input.end()) {
            return input;
        }
    }
    throw SyntaxError("QP-ERROR: invalid name");
}

string QueryTokenizer::getTokenBeforeX(string input, char x, size_t startPos, size_t &nextPos) {
    size_t xPos = input.find(x, startPos);
    if (xPos == string::npos) {
        throw SyntaxError("QP-ERROR: missing " + x);
    }

    nextPos = xPos + 1;
    return trim(input.substr(startPos, xPos - startPos));
}

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
