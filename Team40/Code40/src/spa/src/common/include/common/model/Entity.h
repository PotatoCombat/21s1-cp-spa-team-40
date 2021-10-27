#pragma once

using namespace std;

template <typename Id>
class Entity {
public:
    Id getId() {
        return id;
    }

protected:
    Id id;

    explicit Entity(Id id) : id(id) { };
};
