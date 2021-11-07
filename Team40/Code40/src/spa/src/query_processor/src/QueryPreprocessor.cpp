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

        /*********** Parse return synonym ***********/
        retStrings = tokenizer.tokenizeReturnSynonyms(parts.second, clauses);
        if (retStrings.empty()) {
            this->returnBoolean = true;
        }

        parser.parseDeclarations(declPairs);
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
        this->clear();
        return false;

    } catch (ValidityError &ve) {
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
    // check if there is a declared reference "BOOLEAN"
    // add it as a return reference
    if (this->returnBoolean) {
        Reference *returnRef = parser.getReferenceIfDeclared(KEYWORD_BOOLEAN);
        if (returnRef != nullptr) {
            this->returnBoolean = false;
            q.addReturnReference(returnRef->copy());
        }
    }

    for (auto retString : retStrings) {
        Reference *returnRef = parser.parseReturnSynonym(retString);
        q.addReturnReference(returnRef);
    }
}
