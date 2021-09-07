using namespace std;

#include "Entity.h"
#include "NamedEntity.h"

class NamedStatement : public NamedEntity {
public:
    NamedStatement(string name) : NamedEntity(name) {}
	EntityType getType() {
		return EntityType::STATEMENT;
	}
};
