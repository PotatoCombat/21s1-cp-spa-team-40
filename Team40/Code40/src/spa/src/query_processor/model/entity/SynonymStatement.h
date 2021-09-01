using namespace std;

#include "Entity.h"
#include "SynonymEntity.h"

class SynonymStatement : public SynonymEntity {
public:
    SynonymStatement(string name) : SynonymEntity(name) {}
	EntityType GetType() {
		return EntityType::STATEMENT;
	}
};
