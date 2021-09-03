#include "QueryTokenizer.h"
// #include <cstring> // am i able to use c libraries? :(

using namespace std;

// tokenize query into declaration part and the returnSynonym part and the
// clauses part this returns DECLARATION and SELECT_CLAUSE (vector?) tuple?

// then using the DECLARATION from step 1
// tokenize into DESIGN_ENTITY and SYNONYM

// then using the SELECT_CLAUSES from step 1
// tokenize into RETURN_ENTITY, SUCHTHAT_CLAUSE and PATTERN_CLAUSE

// tokenize SUCHTHAT_CLAUSE into (vector of) RELATION

// tokenize PATTERN_CLAUSE into SYNONYM, ENTITY_REF and EXPRESSION_SPEC

// tokenize EXPRESSION_SPEC into EXPRESSION

string QueryTokenizer::trimString(string input) {
    // char* split = strrchr(*(this->query), delimiter);
    string whitespace = " \n\t\r";
    // return first position non-whitespace
    size_t front_w = input.find_first_not_of(whitespace);
    // return last position non-whitespace
    size_t back_w = input.find_last_not_of(whitespace);
    if (back_w != string::npos && front_w != string::npos) {
        input = input.substr(front_w, back_w - front_w + 1);
    }
    else {
        throw "Empty string";
    }
    return input;
}

pair<string, string> QueryTokenizer::splitIntoParts(string queryString) {
    char delimiter = ';';
    string whitespace = " \n\t\r";
    int split = queryString.rfind(delimiter);
    string declaration = queryString.substr(0, split + 1);
    try {
        declaration = trimString(declaration);
    }
    catch (const char* msg) {
        throw "Empty declaration";
    }
    string selectClause = queryString.substr(split + 1);
    try {
        selectClause = trimString(selectClause);
    }
    catch (const char* msg) {
        throw "Empty select clause";
    }
    return make_pair(declaration, selectClause);
}

vector<string> QueryTokenizer::tokenizeDeclaration(string declaration) {
    // split by ;
    // switch statement ( in parser )
    vector<string> decl;
    string::iterator it;
    string::iterator it_b;
    it = declaration.begin();
    it_b = declaration.begin();
    int found = 0;
    for (; it != declaration.end(); ++it) {
        if (*it == ';') {
            decl.push_back(trimString(string(it_b, it)));
            it_b = it + 1;
            found = 1;
        }
    }
    return decl;
}

vector<string> QueryTokenizer::tokenizeClauses(string selecClause) {
    // split by "Select" + SYNONYM + SUCH_THAT clause / PATTERN clause
    // switch statement?
    return vector<string>();
}
