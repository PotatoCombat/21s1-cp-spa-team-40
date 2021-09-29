#include "query_processor/QueryPreprocessor.h"

void QueryPreprocessor::preprocessQuery(const string input, Query &q) {
    pair<string, string> parts = tokenizer.separateQueryString(input);

    vector<DeclPair> declString;
    string retString;
    string clauses;

    /*********** Parse declaration ***********/
    tokenizer.tokenizeDeclarations(parts.first, declString);
    vector<Reference *> refList;
    for (auto x : declString) {
        Reference *r = parser.parseDeclaration(x);
        refList.push_back(r);
    }

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
        throw ValidityError("return entity is undeclared");
    }

    if (clauses.size() == 0) {
        return;
    }

    /*********** Parse clauses ***********/
    vector<ClsTuple> clsStrings;
    vector<PatTuple> patStrings;
    vector<WithTuple> withStrings;

    tokenizer.tokenizeClauses(clauses, clsStrings, patStrings, withStrings);

    vector<Clause *> clsList;
    for (auto x : clsStrings) {
        Clause *cls = parser.parseClause(x, refList);
        clsList.push_back(cls);
        q.addClause(cls);
    }

    vector<PatternClause *> patList;
    for (auto x : patStrings) {
        PatternClause *pat = parser.parsePattern(x, refList);
        patList.push_back(pat);
        q.addPattern(pat);
    }

    // parse with clauses here
    // ...

    for (int i = 0; i < refList.size(); ++i) {
        delete refList[i];
    }

    return;
}
