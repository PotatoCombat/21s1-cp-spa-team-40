#include "QueryPreprocessor.h"

Query QueryPreprocessor::preprocessQuery(const string input) {
    pair<string, string> parts = tokenizer.splitIntoParts(input);
    vector<DeclTuple> declString =
        tokenizer.tokenizeDeclaration(parts.first);
    string retString = tokenizer.tokenizeReturnEntity(parts.second);
    vector<RelTuple> suchThatString =
        tokenizer.tokenizeSuchThatClause(parts.second);
    // vector<PatTuple> patternString = tokenizer.tokenizePatternClause(parts.second);

    Query q;
    
    vector<Reference> refList;
    for (auto x : declString) {
        Reference r = parser.parseDeclaration(x);
        refList.push_back(r);
    }
    
    int found = 0;
    for (auto x : refList) {
        if (retString == x.getValue()) {
            q.setReturnReference(&x);
        }
    }
    if (!found) {
        throw "Return entity undeclared";
    }

    vector<Relation> relList;
    for (auto x : suchThatString) {
        Relation rel = parser.parseRelation(x, refList);
        relList.push_back(rel);
        q.addRelation(&rel);
    }

    // vector<PatternClause> ptCl;
    // for (auto x : patternString) {
    //    ptCl.push_back(parser.parsePatternClause(x));
    //}

    return q;
}
