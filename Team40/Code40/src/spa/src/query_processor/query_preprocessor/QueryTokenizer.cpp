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

QueryTokenizer::QueryTokenizer(string query) { this->query = query; }

pair<string, string> QueryTokenizer::splitIntoParts() {
    char delimiter = ';';
    string whitespace = " ";
    int split = query.rfind(delimiter);
    string declaration = query.substr(0, split);
    // return first position non-whitespace
    size_t front_w = declaration.find_first_not_of(whitespace);
    // return last position non-whitespace
    size_t back_w = declaration.find_last_not_of(whitespace);
    if (back_w != string::npos && front_w != string::npos) {
        declaration = declaration.substr(front_w, back_w - front_w);
    } else {
        throw "Empty declaration ?";
    }
    string selectClause = query.substr(split);
    // return first position non-whitespace
    size_t front_w = selectClause.find_first_not_of(whitespace);
    // return last position non-whitespace
    size_t back_w = selectClause.find_last_not_of(whitespace);
    if (back_w != string::npos && front_w != string::npos) {
        selectClause = selectClause.substr(front_w, back_w - front_w);
    } else {
        throw "Empty select clause ?";
    }
    // char* split = strrchr(*(this->query), delimiter);
    return make_pair(declaration, selectClause);
}

vector<string> QueryTokenizer::tokenizeDeclaration(string declaration) {
    // split by ;
    // switch statement

    vector<string> decl;
    string::iterator it;
    string::iterator it_b;
    it = declaration.begin();
    it_b = declaration.begin();
    for (; it != declaration.end(); ++it) {
        if (*it == ';') {
            decl.push_back(string(it_b, it));
            it_b = it + 1;
        }
    }
    return decl;
}

vector<string> QueryTokenizer::tokenizeClauses(string selecClause) {
    // split by "Select" + SYNONYM + SUCH_THAT clause / PATTERN clause
    // switch statement?
    return vector<string>();
}
