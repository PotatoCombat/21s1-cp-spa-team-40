using namespace std;

#include "Entity.h"

class Statement : public Entity {
	using Entity::Entity;
public:
	EntityType getType() {
		return EntityType::STATEMENT;
	}
};
