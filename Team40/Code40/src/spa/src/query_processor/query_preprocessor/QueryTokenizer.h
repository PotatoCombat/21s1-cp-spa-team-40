#pragma once

#include <string>
#include <vector>

using namespace std;

// typedef string TOKEN;

class QueryTokenizer {
private:
    string trimString(string input);

public:
    // tokenize query into declaration part and the returnSynonym part and the
    // clauses part this returns DECLARATION and RETURN_SYNONYM and CLAUSES
    // (vector?) tuple?

    // then using the DECLARATION from step 1
    // tokenize into DESIGN_ENTITY and SYNONYM

    // then using the CLAUSES from step 1
    // tokenize into SUCHTHAT_CLAUSE and PATTERN_CLAUSE

    // tokenize SUCHTHAT_CLAUSE into (vector of) RELATION

    // tokenize PATTERN_CLAUSE into SYNONYM, ENTITY_REF and EXPRESSION_SPEC

    // tokenize EXPRESSION_SPEC into EXPRESSION

    QueryTokenizer() {}

    pair<string, string> splitIntoParts(string queryString);

    vector<string> tokenizeDeclaration(string declaration);

    pair<string, vector<string>> tokenizeSelectClause(string selectClause);
};
