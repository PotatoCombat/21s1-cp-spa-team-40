using namespace std;

#include "Entity.h"
#include "NamedEntity.h"

<<<<<<< HEAD:Team40/Code40/src/spa/src/query_processor/model/NamedStatement.cpp
class NamedStatement : public NamedEntity {
public:
	using Entity::Entity;
	EntityType getType() {
		return EntityType::STATEMENT;
	}
=======
class Statement : public Entity {
    using Entity::Entity;

public:
    EntityType GetType() { return EntityType::STATEMENT; }
>>>>>>> d8a4891cd78a074e98aec4d29b6de3a26dfdecb4:Team40/Code40/src/spa/src/query_processor/model/entity/Statement.cpp
};
