#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

typedef string INPUT;
typedef int STATEMENT;
typedef vector<STATEMENT> LIST_OF_STMTS;
typedef vector<Entity> LIST_OF_ENTITES;

LIST_OF_STMTS QueryProcessor(string);

// Usage:
// QueryProcessor("stmt s;\nSelect s such that Follows(10, s)");

LIST_OF_STMTS QueryPreprocessor(string input) {
    // regex to dissect input into declaration & clauses & return value
    Query query;

    regex declaration_regex("\b([a-zA-Z0-9\s]+);");

    smatch s;

    LIST_OF_ENTITES v;

    // extract the entity declaration into Entity objects
    if (regex_search(input, s, declaration_regex)) {
        for (size_t i = 0; i < s.size(); ++i) {
            string decl = s[i].str();
            string type = decl.substr(0, decl.find(" "));
            string synonym = decl.substr(decl.find(" "), decl.size());
            cout << decl << "\n";
            if (type == "stmt") {
                Statement stmt(synonym);
                query.addEntity(&stmt);
                v.push_back(stmt);
            }
        }
    }

    regex return_entity_regex("\bSelect ([a-zA-Z0-9]+)\s");

    smatch s1;

    // extract the return entity into Entity object
    if (regex_search(input, s1, return_entity_regex)) {
        string synonym = s1.str(0);
        cout << synonym << "\n";
        Entity e(synonym);
        if (find(v.begin(), v.end(), e) != v.end()) {
            query.setReturnEntity(&e);
        }
    }

    regex follows_regex("\b\bFollows\(([\w]+),\s*([\w]+)\)");

    smatch s2;

    // extract the clause into FollowsRelationship objects
    if (regex_search(input, s2, follows_regex)) {
        string syn1 = s2.str(0);
        string syn2 = s2.str(1);
        cout << syn1 << " " << syn2;
        FollowsRelationship followsRs(syn1, syn2);
        query.addRelationship(&followsRs);
    }

    // call LIST_OF_STMTS QueryEvaluator.evaluate(Query)
    // return LIST_OF_STMTS

    return LIST_OF_STMTS();
}
