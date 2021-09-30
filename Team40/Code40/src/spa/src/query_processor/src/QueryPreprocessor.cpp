#include "query_processor/QueryPreprocessor.h"

void QueryPreprocessor::preprocessQuery(const string input, Query &q) {
    pair<string, string> parts = tokenizer.separateQueryString(input);

    vector<DeclPair> declPairs;
    string retString;
    string clauses;

    /*********** Parse declaration ***********/
    tokenizer.tokenizeDeclarations(parts.first, declPairs);
    parser.parseDeclarations(declPairs);

    /*********** Parse return synonym ***********/
    tokenizer.tokenizeReturnSynonym(parts.second, retString, clauses);

    int found = 0;
    for (auto x : refList) {
        if (retString == x->getValue()) {
            q.setReturnReference(x->copy());
            found = 1;
            break;
        }
    }

    if (!found) {
        parser.clear();
        throw ValidityError("return entity is undeclared");
    }

    if (clauses.size() == 0) {
        parser.clear();
        return;
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

    return;
}
