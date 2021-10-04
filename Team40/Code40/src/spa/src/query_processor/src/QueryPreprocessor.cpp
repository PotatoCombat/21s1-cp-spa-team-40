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
        retStrings = tokenizer.tokenizeReturnSynonym(parts.second, clauses);

        
        for (auto retString : retStrings) {
            bool found = false;
            Reference* returnRef = parser.parseReturnSynonym(retString, found);
            if (found) {
                q.addReturnReference(returnRef);
            } else {
                throw ValidityError("QP-ERROR: return synonym is undeclared");
            }
        }

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

        vector<PatternClause *> patList;
        for (auto x : patStrings) {
            PatternClause *pat = parser.parsePatternClause(x);
            patList.push_back(pat);
            q.addPattern(pat);
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
