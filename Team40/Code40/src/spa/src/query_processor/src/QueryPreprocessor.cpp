#include "QueryPreprocessor.h"

void QueryPreprocessor::preprocessQuery(const string input, Query &q) {
    try {
        pair<string, string> parts = tokenizer.separateDeclaration(input);
        
        vector<DeclPair> declString;
        tokenizer.tokenizeDeclaration(parts.first, declString);

        vector<Reference> refList;
        for (auto x : declString) {
            Reference r = parser.parseDeclaration(x);
            refList.push_back(r);
        }

        string clauses;
        string retString = tokenizer.tokenizeReturn(parts.second, clauses);

        int found = 0;
        for (auto x : refList) {
            if (retString == x.getValue()) {
                q.setReturnReference(&x);
                found = 1;
            }
        }
        if (!found) {
            throw "error"; // undeclared return
        }

        if (clauses.size() == 0) {
            return;
        }

        // has clauses
        vector<ClsTuple> relString;
        vector<PatTuple> patString;

        tokenizer.tokenizeClause(clauses, relString, patString);

        vector<Clause> clsList;
        for (auto x : relString) {
            Clause rel = parser.parseClause(x, refList);
            clsList.push_back(rel);
            q.addClause(&rel);
        }

        return;
    } catch (const char *msg) {
        throw "invalid query";
    }
}
