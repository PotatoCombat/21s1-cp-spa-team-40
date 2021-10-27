#pragma once

using namespace std;

template <typename Name>
class Entity {
public:
    Name getName() {
        return name;
    }

protected:
    Name name;

    explicit Entity(Name name) : name(name) { };
};
