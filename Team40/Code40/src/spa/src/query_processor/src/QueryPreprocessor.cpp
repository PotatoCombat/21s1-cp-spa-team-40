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
        if (retStrings.empty()) {
            returnBoolean = true;
        }

        this->addReturnReferencesToQuery(retStrings, q);

        if (clauses.empty()) {
            this->clear();
            return true;
        }

        /*********** Parse clauses ***********/
        vector<ClsTuple> clsStrings;
        vector<PatTuple> patStrings;
        vector<WithPair> withStrings;

        tokenizer.tokenizeClauses(clauses, clsStrings, patStrings, withStrings);

        for (auto x : clsStrings) {
            Clause *cls = parser.parseSuchThatClause(x);
            q.addClause(cls);
        }

        for (auto x : patStrings) {
            vector<string> pattern = get<2>(x);
            get<2>(x) = tokenizer.tokenizePattern(pattern);
            Clause *pat = parser.parsePatternClause(x);
            q.addClause(pat);
        }

        for (auto x : withStrings) {
            Clause *cls = parser.parseWithClause(x);
            q.addClause(cls);
        }

        this->clear();
        return true;

    } catch (SyntaxError &se) {
        // cout << e.what();
        this->clear();
        return false;

    } catch (ValidityError &ve) {
        // cout << e.what();
        if (returnBoolean) {
            this->clear();
            throw ValidityError("Set results to FALSE");
        }
        this->clear();
        return false;
    }
}

void QueryPreprocessor::clear() { 
    this->returnBoolean = false;
    this->parser.clear();
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
