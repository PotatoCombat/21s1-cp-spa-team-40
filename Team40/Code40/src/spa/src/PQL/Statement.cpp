using namespace std;

#include "Entity.h"

class Statement : public Entity {
public:
	EntityType getType() {
		return EntityType::Statement;
	}
};
