// steps to take:
// split the query into two parts - declaration and the select clause
// process declaration
// process the return synonym
// process clauses
// how to make a state machine?

#include <string>

#include "query_processor/Abstractions.h"
#include "query_processor/exception/SyntaxError.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include <algorithm>

using namespace std;

char SEMICOLON = ';';
char COMMA = ',';
char L_BRACKET = '(';
char R_BRACKET = ')';
char QUOTE = '"';
char UNDERSCORE = '_';
string WHITESPACE_SET = " \n\t\r";
string KEYWORD_SELECT = "Select";
string KEYWORD_SUCH = "such";
string KEYWORD_THAT = "that";
string KEYWORD_PATTERN = "pattern";
string KEYWORD_AND = "and";
string KEYWORD_WITH = "with";

DesignEntityTypeHelper deHelper = DesignEntityTypeHelper();
//ClauseTypeHelper clsHelper = ClauseTypeHelper();

enum CLAUSE_CODES {
    SUCH_THAT_CLAUSE,
    PATTERN_CLAUSE,
    WITH_CLAUSE,
    NULL_CLAUSE
};

enum STATE_CODES {
    READ_TYPE_STATE,
    SUCH_INTERMEDIATE_STATE,
    READ_ARGS_STATE,
    INVALID_STATE
};

//map<enum STATE_CODES, set<enum STATE_CODES>> stateTransitions = {
//    {READ_TYPE_STATE, {READ_ARGS_STATE, SUCH_INTERMEDIATE_STATE}},
//    {SUCH_INTERMEDIATE_STATE, {READ_ARGS_STATE}},
//    {READ_ARGS_STATE, {READ_TYPE_STATE}}
//};

/**
 * Separates a query string into declaration string and select clause string.
 * @param input The query string.
 * @return Pair of strings where first is the declaration string and second is
 * the select clause string.
 */
pair<string, string> separateQueryString(string input) {
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
void tokenizeDeclarations(string input, vector<DeclPair> &decls) {
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

/**
 * Tokenizes the synonyms part of a single declaration.
 * @param input The synonyms string (assumes at least one synonym).
 * @return Vector of synonym strings.
 */
vector<string> tokenizeDeclarationSynonym(string input) {
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

/**
 * Extracts the return synonym from the select clause and passes the remaining
 * of the clause to a string.
 * @param input The select clause string (assumes non-empty string).
 * @param &returnSynonym The return synonym.
 * @param &remaining The remaining of the select clause.
 */
void tokenizeReturnSynonym(string input, string &returnSynonym,
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
    //}
    //
    // size_t nextTokenPos = findNextToken(input, nextWhitespacePos + 1);
    // nextWhitespacePos = findNextWhitespace(input, nextTokenPos);
    // if (nextWhitespacePos == string::npos) {
    //    remaining = "";
    //} else {
    //    remaining = trimL(input.substr(nextWhitespacePos + 1));
    //}
    // returnSynonym = input.substr(nextTokenPos, nextWhitespacePos -
    // nextTokenPos); return;
}

/**
 * Tokenizes each clause into individual clause tuples and adds them to a vector
 * based on what kind of clause, i.e. such that, pattern or with.
 * @param input The clauses string (assumed non-empty).
 * @param &suchThatClauses Vector of such that tuples.
 * @param &patternClauses Vector of pattern tuples.
 * @param &withClauses Vector of with tuples.
 */
void tokenizeClauses(string input, vector<ClsTuple> &suchThatClauses,
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
            switch (type) {
            case SUCH_THAT_CLAUSE: {
                ClsTuple clause;
                whitespacePos = tokenizeSuchThat(input, tokenPos, clause);
                suchThatClauses.push_back(clause);
            }
            case PATTERN_CLAUSE: {
                PatTuple clause;
                whitespacePos = tokenizePattern(input, tokenPos, clause);
                patternClauses.push_back(clause);
            }
            case WITH_CLAUSE: {
                WithTuple clause;
                whitespacePos = tokenizeWith(input, tokenPos, clause);
                withClauses.push_back(clause);
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

/**
 * Tokenizes a such that clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause ClsTuple.
 * @return Position of end of clause.
 */
size_t tokenizeSuchThat(string input, size_t startPos, ClsTuple &clause) {
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

    //if (isClause == 1) {
    //    token2 = removeWhitespaceWithinQuotes(token2);
    //    token3 = removeWhitespaceWithinQuotes(token3);
    //    clss.push_back(make_tuple(token1, token2, token3));
    //}
    //else if (isClause == 2) {
    //    token2 = removeWhitespaceWithinQuotes(token2);
    //    token3 = extractPatternString(token3);
    //    pats.push_back(make_tuple(token1, token2, token3));
    //}
    //else {
    //    throw SyntaxError("something is very wrong");
    //}
}

/**
 * Tokenizes a pattern clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause PatTuple.
 * @return Position of end of clause.
 */
size_t tokenizePattern(string input, size_t startPos, PatTuple &clause) {}

/**
 * Tokenizes a with clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause WithTuple.
 * @return Position of end of clause.
 */
size_t tokenizeWith(string input, size_t startPos, WithTuple &clause) {}

/**
 * Get token starting from a start positon and before a character x.
 * @param input The clauses string.
 * @param startPos The start positon.
 * @param x The character to find.
 * @param &nextPos Position after X.
 * @return The token to retrieve.
 */
string getTokenBeforeX(string input, char x, size_t startPos, size_t& nextPos) {
    size_t xPos = input.find(x, startPos);
    if (xPos == string::npos) {
        throw SyntaxError("QP-ERROR: missing " + x);
    }

    nextPos = xPos + 1;
    return trim(input.substr(startPos, xPos - startPos));
}

string removeWhitespaceWithinQuotes(string input) {
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


/********************* helper functions *********************/

/**
 * Removes whitespace from the front and back of a given string.
 * @param input The input string.
 * @return The trimmed string, or empty string if there is no string to trim.
 */
string trim(string input) {
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
string trimL(string input) {
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
string trimR(string input) {
    size_t lastPos = input.find_last_not_of(WHITESPACE_SET);
    if (lastPos != string::npos) {
        return input.substr(0, lastPos + 1);
    }
    return "";
}

size_t findNextWhitespace(string input, size_t pos) {
    return input.find_first_of(WHITESPACE_SET, pos);
}

size_t findNextToken(string input, size_t pos) {
    return input.find_first_not_of(WHITESPACE_SET, pos);
}

/**
 * Parses valid name. A valid name is one that starts with a LETTER and contains
 * only LETTERs and DIGITs.
 * @param input The string to parse.
 * @return Valid name.
 * @exception SyntaxError if name is invalid.
 */
string parseValidName(string input) {
    if (isalpha(input[0])) {
        auto it = find_if_not(begin(input), end(input), isalnum);
        if (it == input.end()) {
            return input;
        }
    }
    throw SyntaxError("QP-ERROR: invalid name");
}
