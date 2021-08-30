using namespace std;
#include "Factor.h";

//constructor
Factor::Factor(FactorType factorType) :factorType(factorType) {}

//getters
const FactorType Factor::getFactorType() {
	return this->factorType;
}