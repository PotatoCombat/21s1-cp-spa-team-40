#include "Result.h"

void Result::setValid(bool isValid) {
	this->isValid = isValid;
}


void Result::setResultList1(Reference* reference, vector<string> resultList) {
	reference1 = reference;
	resultList1 = resultList;
}

void Result::setResultList2(Reference* reference, vector<string> resultList) {
	reference2 = reference;
	resultList2 = resultList;
}

bool Result::isResultValid() {
	return isValid;
}

Reference* Result::getReference1() {
	return reference1;
}

Reference* Result::getReference2() {
	return reference2;
}

bool Result::hasResultList1() { return getReference1() != NULL; }

bool Result::hasResultList2() { return getReference2() != NULL; }

vector<string> Result::getResultList1() {
	return resultList1;
}

vector<string> Result::getResultList2() {
	return resultList2;
}

bool Result::equals(Result& other) {
    if (this->isResultValid() != other.isResultValid()) {
        return false;
	}
	
	if (this->hasResultList1() != other.hasResultList1()) {
		return false;    
	}

	if (this->hasResultList2() != other.hasResultList2()) {
        return false;
	}

	if (this->hasResultList1()) {
		if (!(this->getReference1()->equals(*(other.getReference1())) &&
			this->getResultList1() == other.getResultList1())) {
            return false;
		}
	}

	if (this->hasResultList2()) {
		if (!(this->getReference2()->equals(*(other.getReference2())) &&
			this->getResultList2() == other.getResultList2())) {
			return false;
		}
	}

    return true;
}
