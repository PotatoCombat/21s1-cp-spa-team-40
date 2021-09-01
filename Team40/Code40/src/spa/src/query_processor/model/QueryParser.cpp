#include "QueryParser.h";
#include <string>
#include <vector>

vector<Entity *> ExtractDeclarationEntities(string input) {
    // extract the entity declaration into Entity objects
    smatch s;

    vector<Entity *> entities;
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
}

Entity *ExtractReturnEntity(string input) {
    smatch s;

    // extract the return entity into Entity object
    if (regex_search(input, s, return_entity_regex)) {
        string synonym = s.str(0);
        cout << synonym << "\n";
        Entity e(synonym);
        if (find(v.begin(), v.end(), e) != v.end()) {
            query.setReturnEntity(&e);
        }
    }
}

vector<Relationship *> ExtractClauses(string input) {
    smatch s;

    // extract the clause into FollowsRelationship objects
    if (regex_search(input, s, follows_regex)) {
        string syn1 = s.str(0);
        string syn2 = s.str(1);
        cout << syn1 << " " << syn2;
        FollowsRelationship followsRs(syn1, syn2);
        query.addRelationship(&followsRs);
    }
}