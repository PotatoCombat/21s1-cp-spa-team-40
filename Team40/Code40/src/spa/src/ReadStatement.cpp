using namespace std;
#include "Statment.h"
using Statment::Statment;

class ReadStatement : public Statment {

private:
	StatementType type;
	int index;
	vector<char> varsList;
	vector<int> constsList;

public:
	StatementType getStatementType() {
		return StatementType::READ;
	}
};