using namespace std;

#include "Entity.h"

class Statement : public Entity {
    using Entity::Entity;

public:
    EntityType GetType() { return EntityType::STATEMENT; }
};
