#pragma once

#include <regex>
#include <string>
#include <vector>

using namespace std;

class QueryParser {
private:
    const regex declaration_regex("\b([a-zA-Z0-9\s]+);");
    const regex return_entity_regex("\bSelect ([a-zA-Z0-9]+)\s");
    const regex follows_regex("\b\bFollows\(([\w]+),\s*([\w]+)\)");

public:
    QueryParser() {}
    vector<Entity *> ExtractDeclarationEntities(string input);
    Entity *ExtractReturnEntity(string input);
    vector<Relationship *> ExtractClauses(string input);
};
