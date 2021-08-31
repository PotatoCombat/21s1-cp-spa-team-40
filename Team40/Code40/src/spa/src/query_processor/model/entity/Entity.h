#pragma once

#include <string>

using namespace std;

enum class EntityType { UNKNOWN, STATEMENT };

class Entity {
private:
    string name;

public:
    Entity(string name);
    string GetName();
    virtual EntityType GetType();
    bool operator==(const Entity &other);
};
