#include "QueryPreprocessor.h"
#include "../model/Query.h"
#include "../model/entity/SynonymEntity.h"
#include "../model/relationship/FollowsRelationship.cpp"

QueryPreprocessor::QueryPreprocessor() {}

Query QueryPreprocessor::preprocessQuery(const string input) {
    pair<string, string> parts = tokenizer.splitIntoParts(input);
    vector<pair<string, string>> declString =
        tokenizer.tokenizeDeclaration(parts.first);
    string retString = tokenizer.tokenizeReturnEntity(parts.second);
    vector<tuple<string, string, string>> suchThatString =
        tokenizer.tokenizeSuchThatClause(parts.second);
    // vector<tuple<string, string, string>> patternString = tokenizer.tokenizePatternClause(parts.second);

    Query q;
    q.SetReturnEntity(&SynonymEntity(retString));

    //vector<Declaration> decl;
    for (auto x : declString) {
        Declaration d = parser.parseDeclaration(x);
        //decl.push_back(d);
        q.AddEntity(&SynonymEntity(d.getSynonym()));
    }

    //vector<SuchThatClause> stCl;
    for (auto x : suchThatString) {
        SuchThatClause cl = parser.parseSuchThatClause(x);
        //stCl.push_back(cl);
        Entity e1 = Entity(cl.getRelation().getFirstReference().getValue());
        Entity e2 = Entity(cl.getRelation().getSecondReference().getValue());
        Relationship* ship;
        if (cl.getRelation().getRelationType() == RelationType::FOLLOWS) {
            FollowsRelationship fls(&e1, &e2);
            ship = &fls;
        } else {
            throw "Query: error making relationship object";
        }
        q.AddRelationship(ship);
    }

    // vector<PatternClause> ptCl;
    // for (auto x : patternString) {
    //    ptCl.push_back(parser.parsePatternClause(x));
    //}

    return q;
}
