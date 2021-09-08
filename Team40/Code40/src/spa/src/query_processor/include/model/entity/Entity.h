#pragma once

#include <string>

using namespace std;

enum class EntityType { UNKNOWN, STATEMENT };

class Entity {
private:
    string name;

public:
    static const string WILDCARD;
    Entity(string name);
    string getName();
    virtual EntityType getType();
    virtual bool isSynonymEntity();
    bool equals(Entity &other);
};
