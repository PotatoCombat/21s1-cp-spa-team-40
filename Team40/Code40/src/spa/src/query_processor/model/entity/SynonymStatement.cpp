using namespace std;

#include "Entity.h"
#include "SynonymEntity.h"

class SynonymStatement : public SynonymEntity {
public:
	using SynonymEntity::
	EntityType getType() {
		return EntityType::STATEMENT;
	}
};