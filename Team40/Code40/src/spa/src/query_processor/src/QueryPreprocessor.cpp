#include "query_processor/QueryPreprocessor.h"

void QueryPreprocessor::preprocessQuery(const string input, Query &q) {
    pair<string, string> parts;
    vector<DeclPair> declString;
    string clauses;
    string retString;
    parts = tokenizer.separateDeclaration(input);
    tokenizer.tokenizeDeclaration(parts.first, declString);
    retString = tokenizer.tokenizeReturn(parts.second, clauses);

    vector<Reference *> refList; // need to handle deleting of these objects
    for (auto x : declString) {
        Reference *r = parser.parseDeclaration(x);
        refList.push_back(r);
    }

    int found = 0;
    for (auto x : refList) {
        if (retString == x->getValue()) {
            q.setReturnReference(x->copy());
            found = 1;
            break;
        }
    }
    if (!found) {
        throw PreprocessorException("return entity is undeclared");
    }

    if (clauses.size() == 0) {
        return;
    }

    // has clauses
    vector<ClsTuple> clsString;
    vector<PatTuple> patString;

    tokenizer.tokenizeClause(clauses, clsString, patString);

    vector<Clause *> clsList;
    for (auto x : clsString) {
        Clause *cls = parser.parseClause(x, refList);
        clsList.push_back(cls);
        q.addClause(cls);
    }

    vector<PatternClause *> patList;
    for (auto x : patString) {
        PatternClause *pat = parser.parsePattern(x, refList);
        patList.push_back(pat);
        q.addPattern(pat);
    }

    for (int i = 0; i < refList.size(); ++i) {
        delete refList[i];
    }

    return;
}
