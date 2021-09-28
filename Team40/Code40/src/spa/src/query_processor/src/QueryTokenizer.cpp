// steps to take:
// split the query into two parts - declaration and the select clause
// process declaration
// process the return synonym
// process clauses
// how to make a state machine?

#include <string>

#include "query_processor/Abstractions.h";
#include "query_processor/exception/SyntaxError.h";

using namespace std;

char SEMICOLON = ';';
string WHITESPACE_SET = " \n\t\r";
string KEYWORD_SELECT = "Select";

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
 * @param input The declarations string.
 * @param &decls Vector of declaration pairs.
 */
void tokenizeDeclarations(string input, vector<DeclPair> &decls) {
    
}

/**
 * Extracts the return synonym from the select clause and passes the remaining
 * of the clause to a string.
 * @param input The select clause string.
 * @param &returnSynonym The return synonym.
 * @param &remaining The remaining of the select clause.
 */
void tokenizeReturnSynonym(string input, string& returnSynonym, string& remaining) {
    size_t nextWhitespacePos = findNextWhitespace(input, 0);
    string keyword = input.substr(0, nextWhitespacePos);
    if (nextWhitespacePos == string::npos || keyword != KEYWORD_SELECT) {
        throw SyntaxError("QP-ERROR: no select clause");
    }
    
    string rest = trimL(input.substr(nextWhitespacePos + 1));

    nextWhitespacePos = findNextWhitespace(rest, 0);

    if (nextWhitespacePos == string::npos) {
        remaining = "";
        returnSynonym = rest;
        return;
    }

    returnSynonym = rest.substr(0, nextWhitespacePos);
    remaining = trimL(rest.substr(nextWhitespacePos + 1));
    return;

    //size_t nextWhitespacePos = findNextWhitespace(input, 0);
    //string keyword = input.substr(0, nextWhitespacePos);
    //if (nextWhitespacePos == string::npos || keyword != KEYWORD_SELECT) {
    //    throw SyntaxError("QP-ERROR: no select clause");
    //}
    //
    //size_t nextTokenPos = findNextToken(input, nextWhitespacePos + 1);
    //nextWhitespacePos = findNextWhitespace(input, nextTokenPos);
    //if (nextWhitespacePos == string::npos) {
    //    remaining = "";
    //} else {
    //    remaining = trimL(input.substr(nextWhitespacePos + 1));
    //}
    //returnSynonym = input.substr(nextTokenPos, nextWhitespacePos - nextTokenPos);
    //return;
}


/**
 * Tokenizes each clause into individual clause tuples and adds them to a vector based on what kind of clause, i.e. such that, pattern or with.
 * @param input The clauses string.
 * @param &suchThatClauses Vector of such that tuples.
 * @param &patternClauses Vector of pattern tuples.
 * @param &withClauses Vector of with tuples.
 */
void tokenizeClauses(string input, 
                     vector<ClsTuple>& suchThatClauses, 
                     vector<PatTuple>& patternClauses, 
                     vector<WithTuple>& withClauses) {

}

/**
 * Tokenizes a such that clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause ClsTuple.
 */
void tokenizeSuchThat(string input, size_t startPos, ClsTuple& clause) {}

/**
 * Tokenizes a pattern clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause PatTuple.
 */
void tokenizePattern(string input, size_t startPos, PatTuple& clause) {}

/**
 * Tokenizes a with clause into a tuple.
 * @param input The clauses string.
 * @param startPos The start of the clause.
 * @param &clause WithTuple.
 */
void tokenizeWith(string input, size_t startPos, WithTuple& clause) {}


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
