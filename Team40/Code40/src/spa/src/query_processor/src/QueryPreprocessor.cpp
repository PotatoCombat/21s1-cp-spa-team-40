#include "query_processor/QueryPreprocessor.h"

/**
 * Preprocesses a query string and stores the query in an object.
 * @param input The query string.
 * @param q The query object.
 * @return true if preprocessed successfully, false otherwise.
 */
bool QueryPreprocessor::preprocessQuery(const string input, Query &q) {
    try {
        pair<string, string> parts = tokenizer.separateQueryString(input);

        vector<DeclPair> declPairs;
        vector<string> retStrings;
        string clauses;

        /*********** Parse declaration ***********/
        tokenizer.tokenizeDeclarations(parts.first, declPairs);
        parser.parseDeclarations(declPairs);

        /*********** Parse return synonym ***********/
        retStrings = tokenizer.tokenizeReturnSynonyms(parts.second, clauses);

        addReturnReferencesToQuery(retStrings, q);

        if (clauses.empty()) {
            parser.clear();
            return true;
        }

        /*********** Parse clauses ***********/
        vector<ClsTuple> clsStrings;
        vector<PatTuple> patStrings;
        vector<WithTuple> withStrings;

        tokenizer.tokenizeClauses(clauses, clsStrings, patStrings, withStrings);

        vector<Clause *> clsList;
        for (auto x : clsStrings) {
            Clause *cls = parser.parseSuchThatClause(x);
            clsList.push_back(cls);
            q.addClause(cls);
        }

        vector<Clause *> patList;
        for (auto x : patStrings) {
            Clause *pat = parser.parsePatternClause(x);
            patList.push_back(pat);
            q.addClause(pat);
        }

        // parse with clauses here
        // ...

        parser.clear();
        return true;

    } catch (invalid_argument e) {
        // cout << e.what();
        parser.clear();
        return false;
    }
}

void QueryPreprocessor::addReturnReferencesToQuery(vector<string> retStrings,
                                                  Query &q) {
    for (auto retString : retStrings) {
        Reference *returnRef = parser.parseReturnSynonym(retString);
        if (returnRef == nullptr) {
            throw ValidityError("QP-ERROR: return synonym is undeclared");
        }
        q.addReturnReference(returnRef);
    }
}
