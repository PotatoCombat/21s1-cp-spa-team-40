#include "../Reference.h"

using namespace std;

class Relation {
private:
    Reference ref1;
    Reference ref2;

public:
    Relation(Reference ref1, Reference ref2);
    Reference getFirstReference();
    Reference getSecondReference();
    RelationType getRelationType();
};
