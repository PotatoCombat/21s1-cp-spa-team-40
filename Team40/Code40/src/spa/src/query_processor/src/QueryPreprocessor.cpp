#include "QueryPreprocessor.h"

void QueryPreprocessor::preprocessQuery(const string input, Query &q) {
    try {
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
                q.setReturnReference(x);
                found = 1;
                break;
            }
        }
        if (!found) {
            throw "error"; // undeclared return
        }

        if (clauses.size() == 0) {
            return;
        }

        /*for (int i = 0; i < refList.size(); ++i) {
            delete refList[i];
        }*/

        // has clauses
        vector<ClsTuple> relString;
        vector<PatTuple> patString;

        tokenizer.tokenizeClause(clauses, relString, patString);

        vector<Clause *> clsList;
        for (auto x : relString) {
            Clause *rel = parser.parseClause(x, refList);
            clsList.push_back(rel);
            q.addClause(rel);
        }

        return;
    } catch (const char *msg) {
        throw "invalid query";
    }
}
