#pragma once

#include <string>

using namespace std;

enum class EntityType { STATEMENT };

class Entity {
private:
    string name;

public:
    Entity(string name);
    string GetName();
    virtual EntityType GetType() = 0;
    virtual bool isSynonymEntity() = 0;
    bool operator==(const Entity &other);
};
